#include <string>
#include <range/v3/algorithm/for_each.hpp>

#include "flappy/scenes/FlappyBirdScene.hpp"
#include "flappy/sprites/FlappyBird.hpp"
#include "flappy/sprites/Obstacle.hpp"
#include "flappy/utilities/Geometry.hpp"
#include "flappy/utilities/Random.hpp"

using namespace cocos2d;
using namespace flappy;

Scene* GameScene::createScene() {
    const auto scene = Scene::create();
    const auto layer = FlappyBirdScene::create();
    scene->addChild(layer);
    return scene;
}

bool FlappyBirdScene::init() {
    if (!GameScene::init()) {
        return false;
    }

    addMenuOptions();
    addScoreLabel();
    addTouchListeners();
    initScene();
    scheduleUpdate();

    return true;
}

void FlappyBirdScene::initScene() {
    GameScene::initScene();

    updateScore();
    addFlappy();
    generateObstacles();
}

void FlappyBirdScene::clearScene() {
    GameScene::clearScene();

    stopAllActions();
    const auto removeObstacle = [this](auto obstacle) { this->removeChild(obstacle); };
    ranges::for_each(passedObstacles, removeObstacle);
    ranges::for_each(incomingObstacles, removeObstacle);
    passedObstacles.clear();
    incomingObstacles.clear();

    removeChild(flappy);
    gameState.reset();
}

void FlappyBirdScene::addMenuOptions() {
    const auto pauseItem = MenuItemImage::create("CloseNormal.png", "CloseSelected.png", CC_CALLBACK_1(FlappyBirdScene::onMenuPause, this));
    pauseItem->setAnchorPoint(Vec2(1.0, 1.0));
    pauseItem->setPosition(Vec2(frame.origin.x + frame.size.width - 20, frame.origin.y + frame.size.height - 20));
    const auto menu = Menu::create(pauseItem, nullptr);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);
}

void FlappyBirdScene::addScoreLabel() {
    scoreLabel = Label::createWithSystemFont("", "Arial", 24);
    scoreLabel->setAnchorPoint(Vec2(0, 1.0));
    scoreLabel->setPosition(Vec2(frame.origin.x + 20, frame.origin.y + frame.size.height - 20));
    this->addChild(scoreLabel, 1);
}

void FlappyBirdScene::addFlappy() {
    flappy = FlappyBird::create();
    flappy->setPosition(geometry::centerOf(frame));
    addChild(flappy, 1);
}

Obstacle* FlappyBirdScene::generateObstacle() const {
    const auto maxColumnHeight = frame.size.height - Obstacle::gapHeight;
    const auto bottomHeight = random::between(0, maxColumnHeight);
    const auto topHeight = maxColumnHeight - bottomHeight;

    const auto obstacle = Obstacle::create(topHeight, Obstacle::gapHeight, bottomHeight);
    obstacle->setPosition(geometry::rightOf(obstacle->getContentSize(), frame));
    return obstacle;
}

void FlappyBirdScene::addObstacle() {
    const auto onCompletion = [this](auto obstacle) { passedObstacles.remove(obstacle); };
    const auto obstacle = generateObstacle();
    obstacle->runActions(onCompletion);
    addChild(obstacle);
    incomingObstacles.emplace_back(obstacle);
}

void FlappyBirdScene::generateObstacles() {
    const auto delay = DelayTime::create(2);
    const auto generateNewColumn = CallFunc::create([this]() { addObstacle(); });
    const auto delayedColumnGenerator = Sequence::create(generateNewColumn, delay, nullptr);
    const auto infiniteColumnGenerator = RepeatForever::create(delayedColumnGenerator);
    runAction(infiniteColumnGenerator);
}

std::optional<Obstacle*> FlappyBirdScene::nearestObstacle() const {
    return incomingObstacles.empty() ? std::nullopt : std::make_optional(incomingObstacles.front());
}

void FlappyBirdScene::update(float dt) {
    flappy->update(dt);

    const auto possibleNearestObstacle = nearestObstacle();
    if (!possibleNearestObstacle) {
        return;
    }

    const auto obstacle = *possibleNearestObstacle;
    const auto flappyFrame = flappy->getBoundingBox();
    const auto flappyIsOnScreen = flappyFrame.intersectsRect(getFrame());
    if (obstacle->collidesWith(flappyFrame) || !flappyIsOnScreen) {
        GameScene::stopScene();
    } else if (obstacle->passedBy(flappyFrame)) {
        gameState.addToScore();
        updateScore();
        incomingObstacles.pop_front();
        passedObstacles.emplace_back(obstacle);
    }
}

void FlappyBirdScene::updateScore() {
    scoreLabel->setString("Score: " + std::to_string(gameState.currentScore()));
}

void FlappyBirdScene::addTouchListeners() {
    const auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = CC_CALLBACK_2(FlappyBirdScene::onTouchBegan, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

bool FlappyBirdScene::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event) {
    switch (sceneStatus()) {
        case GameScene::Status::Running:
            flappy->velocity.y = 850;
            return true;
        case GameScene::Status::Paused:
            GameScene::resumeScene();
            return false;
        case GameScene::Status::Stopped:
            GameScene::resetScene();
            return false;
    }
}

void FlappyBirdScene::onMenuPause(cocos2d::Ref* menuItem) {
    switch (sceneStatus()) {
        case GameScene::Status::Running:
            GameScene::pauseScene();
            break;
        case GameScene::Status::Paused:
            GameScene::resumeScene();
            break;
        case GameScene::Status::Stopped:
            GameScene::exitScene();
            break;
    }
}
