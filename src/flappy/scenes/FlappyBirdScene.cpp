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

    addFlappy();
    generateObstacles();
    addTouchListeners();
    addKeyboardListeners();
    scheduleUpdate();

    return true;
}

void FlappyBirdScene::addFlappy() {
    flappy = FlappyBird::create();
    flappy->setPosition(geometry::centerOf(frame));
    addChild(flappy, 1);
}

Obstacle* FlappyBirdScene::generateObstacle() {
    const auto maxColumnHeight = frame.size.height - Obstacle::gapHeight;
    const auto bottomHeight = random::between(0, maxColumnHeight);
    const auto topHeight = maxColumnHeight - bottomHeight;

    const auto obstacle = Obstacle::create(topHeight, Obstacle::gapHeight, bottomHeight);
    obstacle->setPosition(geometry::rightOf(obstacle->getContentSize(), frame));
    return obstacle;
}

void FlappyBirdScene::addObstacle() {
    const auto onCompletion = [this](const auto obstacle) { passedObstacles.pop_back(); };
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

std::optional<const Obstacle*> FlappyBirdScene::nearestObstacle() {
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
        log("Hit!");
        score = 0;
        GameScene::pauseScene();
    } else if (obstacle->passedBy(flappyFrame)) {
        ++score;
        log("Score: %d", score);
        incomingObstacles.pop_front();
        passedObstacles.emplace_back(obstacle);
    }
}

void FlappyBirdScene::addTouchListeners() {
    const auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = CC_CALLBACK_2(FlappyBirdScene::onTouchBegan, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void FlappyBirdScene::addKeyboardListeners() {
    const auto listener = EventListenerKeyboard::create();
    listener->onKeyPressed = [this](auto keycode, auto event) {
        if (keycode == EventKeyboard::KeyCode::KEY_SPACE) {
            GameScene::isScenePaused() ? GameScene::resumeScene() : GameScene::pauseScene();
        }
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

bool FlappyBirdScene::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event) {
    flappy->velocity.y = 850;
    return true;
}
