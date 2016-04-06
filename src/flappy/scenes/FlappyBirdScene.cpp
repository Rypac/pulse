#include <string>
#include <range/v3/algorithm/for_each.hpp>
#include <range/v3/view/filter.hpp>
#include <range/v3/to_container.hpp>

#include "flappy/scenes/FlappyBirdScene.hpp"
#include "flappy/sprites/FlappyBird.hpp"
#include "flappy/sprites/Obstacle.hpp"
#include "flappy/sprites/SpritePhysicsBody.hpp"
#include "flappy/sprites/ObstaclePhysicsBody.hpp"
#include "flappy/generators/ObstacleGenerator.hpp"
#include "flappy/models/Accelerometer.hpp"
#include "flappy/utilities/Geometry.hpp"
#include "flappy/utilities/Physics.hpp"
#include "flappy/utilities/Rotation.hpp"

using namespace cocos2d;
using namespace flappy;

FlappyBirdScene* FlappyBirdScene::create(const GameOptions& options) {
    FlappyBirdScene *scene = new (std::nothrow) FlappyBirdScene{options};
    if (scene && scene->init()) {
        scene->autorelease();
        return scene;
    }
    CC_SAFE_DELETE(scene);
    return nullptr;
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
    addGameStateListeners();
    setupScene();

    return true;
}

void FlappyBirdScene::setupScene() {
    updateScore();
    addFlappy();
}

void FlappyBirdScene::clearScene() {
    stopAllActions();
    removeChild(flappy);
    ranges::for_each(obstacles, [this](auto obstacle) {
        this->removeChild(obstacle);
    });
    obstacles.clear();
    gameState.reset();
}

void FlappyBirdScene::resetScene() {
    stopScene();
    clearScene();
    setupScene();
    runScene();
}

void FlappyBirdScene::runScene() {
    if (gameState.isGameOver()) {
        return;
    }

    GameScene::runScene();
    scheduleUpdate();
    scheduleObstacleGeneration();
    updateSceneTimeScale();
}

void FlappyBirdScene::stopScene() {
    GameScene::stopScene();
    unscheduleObstacleGeneration();
}

void FlappyBirdScene::addMenuOptions() {
    const auto onPause = [this](auto ref) {
        if (onEnterMenu) {
            onEnterMenu(this);
        }
    };
    const auto pauseImage = Sprite::create();
    pauseImage->setColor(Color3B::GREEN);
    pauseImage->setContentSize(Size{40, 40});
    pauseImage->setTextureRect(pauseImage->getBoundingBox());
    const auto pauseItem = MenuItemSprite::create(pauseImage, pauseImage, onPause);
    pauseItem->setAnchorPoint(Vec2{1.0, 1.0});
    pauseItem->setPosition(Vec2{frame.origin.x + frame.size.width - 15, frame.origin.y + frame.size.height - 15});
    const auto menu = Menu::create(pauseItem, nullptr);
    menu->setPosition(Vec2::ZERO);
    addChild(menu, 1);
}

void FlappyBirdScene::addScoreLabel() {
    scoreLabel = Label::createWithTTF("", "fonts/Arial.ttf", 24);
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

void FlappyBirdScene::addObstacle() {
    const auto obstacle = ObstacleGenerator{frame}.generate();
    obstacle->onCompletion = [this](auto obstacle) { obstacles.remove(obstacle); };
    obstacle->setPhysicsBody(ObstaclePhysicsBody::create(obstacle));
    addChild(obstacle);
    obstacles.emplace_back(obstacle);
    obstacle->runActions(options.obstacleSpeed);
}

void FlappyBirdScene::scheduleObstacleGeneration() {
    const auto delay = DelayTime::create(options.obstacleFrequency);
    const auto generateObstacle = CallFunc::create([this]() { addObstacle(); });
    const auto reschedule = CallFunc::create([this]() { scheduleObstacleGeneration(); });
    obstacleGenerator = Sequence::create(delay, generateObstacle, reschedule, nullptr);
    runAction(obstacleGenerator);
}

void FlappyBirdScene::unscheduleObstacleGeneration() {
    if (obstacleGenerator) {
        stopAction(obstacleGenerator);
    }
}

void FlappyBirdScene::update(float dt) {
    getScene()->getPhysicsWorld()->step(dt);

    if (!residesInSceneBounds(*flappy)) {
        handleGameOver();
    }
}

void FlappyBirdScene::updateScore() {
    scoreLabel->setString("Score: " + std::to_string(gameState.currentScore()));
}

void FlappyBirdScene::updateSceneTimeScale() {
    getScene()->getScheduler()->setTimeScale(gameState.environmentTimeScale());
}

void FlappyBirdScene::addTouchListeners() {
    const auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = CC_CALLBACK_2(FlappyBirdScene::onTouchBegan, this);
    listener->onTouchEnded = CC_CALLBACK_2(FlappyBirdScene::onTouchEnded, this);
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
    _eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);
}

void FlappyBirdScene::addGameStateListeners() {
    gameState.onTimeModeChanged = [this](auto mode) {
        this->updateSceneTimeScale();
        if (mode == GameState::TimeMode::Normal) {
            this->handleDefeatedObstacles();
        }
    };
}

bool FlappyBirdScene::onTouchBegan(Touch* touch, Event* event) {
    if (sceneStatus() == GameScene::Status::Running) {
        gameState.enterMode(GameState::TimeMode::SlowMotion);
        return true;
    } else {
        resetScene();
        return false;
    }
}

void FlappyBirdScene::onTouchEnded(Touch* touch, Event* event) {
    if (sceneStatus() == GameScene::Status::Running) {
        gameState.enterMode(GameState::TimeMode::Normal);
    }
}

void FlappyBirdScene::onAccelerationDetected(Acceleration* acceleration, Event* event) {
    if (sceneStatus() == GameScene::Status::Stopped) {
        return;
    }

    const auto currentAcceleration = Vec3(acceleration->x, acceleration->y, acceleration->z);
    auto& accelerometer = gameState.accelerometer();
    if (!accelerometer.isCalibrated()) {
        accelerometer.calibrate(rotation::angle(currentAcceleration));
        return;
    }

    const auto offset = *accelerometer.offset();
    const auto x = rotation::roll(currentAcceleration) - offset.x;
    const auto y = rotation::pitch(currentAcceleration) - offset.y;
    auto velocity = Vec2{x, y};
    velocity.scale(options.accelerometerSensitivity);
    flappy->getPhysicsBody()->setVelocity(velocity * gameState.playerTimeScale());
}

bool FlappyBirdScene::onContactBegan(PhysicsContact &contact) {
    if (physics_body::collision::heroAndObstacle(contact)) {
        handleGameOver();
    } else if (physics_body::collision::heroAndPath(contact)) {
        const auto path = *physics_body::nodeInContact(contact, physics_body::isPath);
        const auto obstacle = static_cast<Obstacle*>(path->getParent());
        handlePassedObstacle(obstacle);
    }
    return false;
}

void FlappyBirdScene::handleGameOver() {
    gameState.gameOver();
    stopScene();
}

void FlappyBirdScene::handlePassedObstacle(Obstacle* obstacle) {
    gameState.incrementScore();
    updateScore();
    obstacle->getPhysicsBody()->pass();

    if (gameState.currentMode() == GameState::TimeMode::Normal) {
        handleDefeatedObstacle(obstacle);
    }
}

void FlappyBirdScene::handleDefeatedObstacle(Obstacle* obstacle) {
    obstacle->getPhysicsBody()->defeat();
    obstacle->runDefeatedActions();
}

void FlappyBirdScene::handleDefeatedObstacles() {
    const auto obstacleWasPassed = [](Obstacle* obstacle) {
        return obstacle->getPhysicsBody()->currentState() == ObstacleState::Passed;
    };
    const auto removeObstacle = [this](Obstacle* obstacle) {
        this->handleDefeatedObstacle(obstacle);
    };
    const auto passedObstacles = obstacles | ranges::view::filter(obstacleWasPassed) | ranges::to_vector;
    ranges::for_each(passedObstacles, removeObstacle);
}
