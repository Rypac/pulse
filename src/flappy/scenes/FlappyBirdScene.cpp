#include <string>
#include <range/v3/algorithm/for_each.hpp>
#include <range/v3/algorithm/any_of.hpp>

#include "flappy/scenes/FlappyBirdScene.hpp"
#include "flappy/sprites/FlappyBird.hpp"
#include "flappy/sprites/Obstacle.hpp"
#include "flappy/utilities/Geometry.hpp"
#include "flappy/utilities/Physics.hpp"
#include "flappy/generators/ObstacleGenerator.hpp"
#include "flappy/utilities/Rotation.hpp"

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
    addAccelerometerListeners();

    return true;
}

void FlappyBirdScene::initScene() {
    GameScene::initScene();

    updateScore();
    addFlappy();
    generateObstacles();
    scheduleUpdate();
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
    pauseItem->setAnchorPoint(Vec2{1.0, 1.0});
    pauseItem->setPosition(Vec2{frame.origin.x + frame.size.width - 20, frame.origin.y + frame.size.height - 20});
    const auto menu = Menu::create(pauseItem, nullptr);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);
}

void FlappyBirdScene::addScoreLabel() {
    scoreLabel = Label::createWithSystemFont("", "Arial", 24);
    scoreLabel->setAnchorPoint(Vec2{0, 1.0});
    scoreLabel->setPosition(Vec2{frame.origin.x + 20, frame.origin.y + frame.size.height - 20});
    this->addChild(scoreLabel, 1);
}

void FlappyBirdScene::addFlappy() {
    flappy = FlappyBird::create();
    flappy->setPosition(geometry::centerOf(frame));
    addChild(flappy, 1);
}

void FlappyBirdScene::addObstacle() {
    const auto onCompletion = [this](auto obstacle) { passedObstacles.remove(obstacle); };
    const auto obstacle = ObstacleGenerator{frame}.generate();
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

std::optional<Obstacle*> FlappyBirdScene::closestIncomingObstacle() const {
    return incomingObstacles.empty() ? std::nullopt : std::make_optional(incomingObstacles.front());
}

void FlappyBirdScene::update(float dt) {
    flappy->update(dt);

    if (collisionDetected()) {
        GameScene::stopScene();
    } else {
        updateIncomingObstacles();
    }
}

bool FlappyBirdScene::collisionDetected() const {
    return collisionWithEnvironment()
        || collisionWithObstacles(incomingObstacles)
        || collisionWithObstacles(passedObstacles);
}

bool FlappyBirdScene::collisionWithEnvironment() const {
    const auto flappyFrame = flappy->getBoundingBox();
    const auto flappyIsOnScreen = flappyFrame.intersectsRect(getFrame());
    return !flappyIsOnScreen;
}

bool FlappyBirdScene::collisionWithObstacles(const std::list<Obstacle*>& obstacles) const {
    const auto flappyFrame = flappy->getBoundingBox();
    return ranges::any_of(obstacles, [=](const auto& obstacle) {
        return obstacle->collidesWith(flappyFrame);
    });
}

void FlappyBirdScene::updateIncomingObstacles() {
    const auto possibleNearestObstacle = closestIncomingObstacle();
    if (!possibleNearestObstacle) {
        return;
    }

    const auto obstacle = *possibleNearestObstacle;
    if (obstacle->passedBy(flappy->getBoundingBox())) {
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

void FlappyBirdScene::addAccelerometerListeners() {
    Device::setAccelerometerEnabled(true);
    Device::setAccelerometerInterval(1 / 30);
    const auto listener = EventListenerAcceleration::create(CC_CALLBACK_2(FlappyBirdScene::onAccelerationDetected, this));
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

bool FlappyBirdScene::onTouchBegan(Touch* touch, Event* event) {
    switch (sceneStatus()) {
        case GameScene::Status::Initialising:
        case GameScene::Status::Running:
            return false;
        case GameScene::Status::Paused:
            GameScene::resumeScene();
            return false;
        case GameScene::Status::Stopped:
            GameScene::resetScene();
            return false;
    }
}

void FlappyBirdScene::onAccelerationDetected(Acceleration* acceleration, Event* event) {
    const auto currentAcceleration = Vec3(acceleration->x, acceleration->y, acceleration->z);
    switch (sceneStatus()) {
        case GameScene::Status::Initialising:
            gameState.calibrateAccelerometer(rotation::angle(currentAcceleration));
            initScene();
            break;
        case GameScene::Status::Running: {
            const auto roll = rotation::roll(currentAcceleration) - gameState.calibratedAccelerometerOffset().x;
            const auto pitch = rotation::pitch(currentAcceleration) - gameState.calibratedAccelerometerOffset().y;
            flappy->velocity = Vec2{roll, pitch} * physics::AccelerometerScale;
            break;
        }
        case GameScene::Status::Paused:
            break;
        case GameScene::Status::Stopped:
            gameState.calibrateAccelerometer(rotation::angle(currentAcceleration));
            break;
    }
}

void FlappyBirdScene::onMenuPause(Ref* menuItem) {
    switch (sceneStatus()) {
        case GameScene::Status::Initialising:
            break;
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
