#include <range/v3/algorithm/for_each.hpp>

#include "flappy/scenes/FlappyBirdScene.hpp"
#include "flappy/sprites/FlappyBird.hpp"
#include "flappy/sprites/Obstacle.hpp"
#include "flappy/utilities/Geometry.hpp"
#include "flappy/utilities/Random.hpp"

using namespace cocos2d;
using namespace flappy;

Scene* GameScene::createScene() {
    auto scene = Scene::create();
    auto layer = FlappyBirdScene::create();
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
    gameState.status = GameState::Status::Running;
    scoreLabel->setString("Score: " + std::to_string(gameState.score));
    addFlappy();
    generateObstacles();
}

void FlappyBirdScene::clearScene() {
    stopAllActions();
    const auto removeObstacle = [this](auto obstacle) { removeChild(obstacle); };
    ranges::for_each(passedObstacles, removeObstacle);
    ranges::for_each(incomingObstacles, removeObstacle);
    passedObstacles.clear();
    incomingObstacles.clear();

    removeChild(flappy);
    gameState.reset();
}

void FlappyBirdScene::addMenuOptions() {
    const auto pauseItem = MenuItemImage::create("CloseNormal.png", "CloseSelected.png", CC_CALLBACK_1(FlappyBirdScene::onMenuPause, this));
    pauseItem->setAnchorPoint(Vec2(1.0, 0));
    pauseItem->setPosition(Vec2(frame.origin.x + frame.size.width - 20, frame.origin.y + 20));
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

    auto possibleNearestObstacle = nearestObstacle();
    if (!possibleNearestObstacle) {
        return;
    }

    const auto obstacle = *possibleNearestObstacle;
    const auto flappyFrame = flappy->getBoundingBox();
    if (obstacle->collidesWith(flappyFrame)) {
        gameState.status = GameState::Status::Stopped;
        GameScene::pauseScene();
    } else if (obstacle->passedBy(flappyFrame)) {
        gameState.score++;
        scoreLabel->setString("Score: " + std::to_string(gameState.score));
        incomingObstacles.pop_front();
        passedObstacles.emplace_back(obstacle);
    }
}

void FlappyBirdScene::addTouchListeners() {
    const auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = CC_CALLBACK_2(FlappyBirdScene::onTouchBegan, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

bool FlappyBirdScene::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event) {
    switch (gameState.status) {
        case GameState::Status::Running:
            flappy->velocity.y = 850;
            return true;
        case GameState::Status::Paused:
            gameState.status = GameState::Status::Running;
            GameScene::resumeScene();
            return false;
        case GameState::Status::Stopped:
            GameScene::resetScene();
            return false;
    }
}

void FlappyBirdScene::onMenuPause(cocos2d::Ref* menuItem) {
    switch (gameState.status) {
        case GameState::Status::Running:
            gameState.status = GameState::Status::Paused;
            GameScene::pauseScene();
            break;
        case GameState::Status::Paused:
            gameState.status = GameState::Status::Running;
            GameScene::resumeScene();
            break;
        case GameState::Status::Stopped:
            GameScene::exitScene();
            break;
    }
}
