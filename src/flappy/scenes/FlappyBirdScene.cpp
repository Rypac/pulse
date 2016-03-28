#include <string>
#include <range/v3/algorithm/for_each.hpp>

#include "flappy/scenes/FlappyBirdScene.hpp"
#include "flappy/sprites/FlappyBird.hpp"
#include "flappy/sprites/Obstacle.hpp"
#include "flappy/sprites/SpritePhysicsBody.hpp"
#include "flappy/utilities/Geometry.hpp"
#include "flappy/utilities/Physics.hpp"
#include "flappy/generators/ObstacleGenerator.hpp"
#include "flappy/utilities/Rotation.hpp"

using namespace cocos2d;
using namespace flappy;

Scene* GameScene::createScene() {
    const auto scene = Scene::createWithPhysics();
    scene->getPhysicsWorld()->setGravity(Vec2{0, 0});
    scene->getPhysicsWorld()->setAutoStep(false);
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
    addCollisionListeners();

    return true;
}

void FlappyBirdScene::initScene() {
    GameScene::initScene();

    updateScore();
    addFlappy();
    scheduleUpdate();
    schedule(CC_SCHEDULE_SELECTOR(FlappyBirdScene::addObstacle), 1.5);
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
    addChild(menu, 1);
}

void FlappyBirdScene::addScoreLabel() {
    scoreLabel = Label::createWithSystemFont("", "Arial", 24);
    scoreLabel->setAnchorPoint(Vec2{0, 1.0});
    scoreLabel->setPosition(Vec2{frame.origin.x + 20, frame.origin.y + frame.size.height - 20});
    addChild(scoreLabel, 1);
}

void FlappyBirdScene::addFlappy() {
    flappy = FlappyBird::create(Size{30, 30});
    flappy->setPosition(geometry::centerOf(frame));
    flappy->setPhysicsBody(physics_body::createHero(flappy->getBoundingBox().size));
    addChild(flappy, 1);
}

void addPhysicsToObstacle(const Obstacle& obstacle) {
    obstacle.getTop()->setPhysicsBody(physics_body::createObstacle(obstacle.getTop()->getBoundingBox().size));
    obstacle.getBottom()->setPhysicsBody(physics_body::createObstacle(obstacle.getBottom()->getBoundingBox().size));
    obstacle.getGap()->setPhysicsBody(physics_body::createPath(obstacle.getGap()->getBoundingBox().size));
}

void FlappyBirdScene::addObstacle(float dt) {
    const auto onCompletion = [this](auto obstacle) { incomingObstacles.remove(obstacle); };
    const auto obstacle = ObstacleGenerator{frame}.generate();
    addPhysicsToObstacle(*obstacle);
    addChild(obstacle);
    incomingObstacles.emplace_back(obstacle);
    obstacle->runActions(onCompletion);
}

void FlappyBirdScene::update(float dt) {
    if (sceneStatus() != GameScene::Status::Running) {
        return;
    }

    getScene()->getPhysicsWorld()->step(dt);

    if (!residesInSceneBounds(*flappy)) {
        GameScene::stopScene();
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

void FlappyBirdScene::addCollisionListeners() {
    auto contactListener = EventListenerPhysicsContact::create();
    contactListener->onContactBegin = CC_CALLBACK_1(FlappyBirdScene::onContactBegan, this);
    contactListener->onContactSeparate = CC_CALLBACK_1(FlappyBirdScene::onContactEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);
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
            flappy->getPhysicsBody()->setVelocity(Vec2{roll, pitch} * physics::AccelerometerScale);
            break;
        }
        case GameScene::Status::Paused:
            break;
        case GameScene::Status::Stopped:
            gameState.calibrateAccelerometer(rotation::angle(currentAcceleration));
            break;
    }
}

bool FlappyBirdScene::onContactBegan(PhysicsContact &contact) {
    if (physics_body::collision::heroAndObstacle(contact)) {
        GameScene::stopScene();
    } else if (physics_body::collision::heroAndPath(contact)) {
        gameState.addToScore();
        updateScore();
    }
    return false;
}

void FlappyBirdScene::onContactEnded(PhysicsContact &contact) {

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
