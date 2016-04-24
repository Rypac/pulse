#include <string>
#include <range/v3/algorithm/for_each.hpp>
#include <range/v3/view/filter.hpp>
#include <range/v3/to_container.hpp>

#include "pulse/scenes/PulseGameScene.hpp"
#include "pulse/sprites/Obstacle.hpp"
#include "pulse/sprites/SpritePhysicsBody.hpp"
#include "pulse/sprites/ObstaclePhysicsBody.hpp"
#include "pulse/sprites/WrappedSprite.hpp"
#include "pulse/generators/ObstacleGenerator.hpp"
#include "pulse/models/Accelerometer.hpp"
#include "pulse/utilities/Geometry.hpp"
#include "pulse/utilities/Physics.hpp"
#include "pulse/utilities/Rotation.hpp"
#include "pulse/ui/Font.hpp"

using namespace cocos2d;
using namespace pulse;

PulseGameScene* PulseGameScene::create(const GameOptions& options) {
    PulseGameScene *scene = new (std::nothrow) PulseGameScene{options};
    if (scene && scene->init()) {
        scene->autorelease();
        return scene;
    }
    delete scene;
    return nullptr;
}

bool PulseGameScene::init() {
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

void PulseGameScene::setupScene() {
    updateScore();
    addPlayer();
}

void PulseGameScene::clearScene() {
    stopAllActions();
    removeChild(player);
    ranges::for_each(obstacles, [this](auto obstacle) {
        this->removeChild(obstacle);
    });
    obstacles.clear();
    gameState.reset();
}

void PulseGameScene::resetScene() {
    stopScene();
    clearScene();
    setupScene();
    runScene();
}

void PulseGameScene::runScene() {
    if (gameState.isGameOver()) {
        return;
    }

    GameScene::runScene();
    scheduleUpdate();
    scheduleObstacleGeneration();
    updateSceneTimeScale();
}

void PulseGameScene::stopScene() {
    GameScene::stopScene();
    unscheduleObstacleGeneration();
}

void PulseGameScene::addMenuOptions() {
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

void PulseGameScene::addScoreLabel() {
    scoreLabel = Label::createWithTTF("", Font::Arial, 24);
    scoreLabel->setAnchorPoint(Vec2{0, 1.0});
    scoreLabel->setPosition(Vec2{frame.origin.x + 20, frame.origin.y + frame.size.height - 20});
    addChild(scoreLabel, 1);
}

void PulseGameScene::addPlayer() {
    player = WrappedSprite::create();
    const auto size = Size{30, 30};
    player->setContentSize(size);
    player->setTextureRect(Rect{Vec2::ZERO, size});
    player->setPosition(geometry::centerOf(frame));
    player->setPhysicsBody(physics_body::createHero(player->getBoundingBox().size));
    addChild(player, 1);
}

void PulseGameScene::addObstacle() {
    const auto obstacle = ObstacleGenerator{frame}.generate();
    obstacle->onCompletion = [this](auto obstacle) { obstacles.remove(obstacle); };
    obstacle->setPhysicsBody(ObstaclePhysicsBody::create(obstacle));
    addChild(obstacle);
    obstacles.emplace_back(obstacle);
    obstacle->runActions(options.obstacleSpeed);
}

void PulseGameScene::scheduleObstacleGeneration() {
    const auto delay = DelayTime::create(options.obstacleFrequency);
    const auto generateObstacle = CallFunc::create([this]() { addObstacle(); });
    const auto reschedule = CallFunc::create([this]() { scheduleObstacleGeneration(); });
    obstacleGenerator = Sequence::create(delay, generateObstacle, reschedule, nullptr);
    runAction(obstacleGenerator);
}

void PulseGameScene::unscheduleObstacleGeneration() {
    if (obstacleGenerator) {
        stopAction(obstacleGenerator);
    }
}

void PulseGameScene::update(float dt) {
    getScene()->getPhysicsWorld()->step(dt);

    if (!residesInSceneBounds(*player)) {
        handleGameOver();
    }
}

void PulseGameScene::updateScore() {
    scoreLabel->setString("Score: " + std::to_string(gameState.currentScore()));
}

void PulseGameScene::updateSceneTimeScale() {
    getScene()->getScheduler()->setTimeScale(gameState.environmentTimeScale());
}

void PulseGameScene::addTouchListeners() {
    const auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = CC_CALLBACK_2(PulseGameScene::onTouchBegan, this);
    listener->onTouchEnded = CC_CALLBACK_2(PulseGameScene::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void PulseGameScene::addAccelerometerListeners() {
    Device::setAccelerometerEnabled(true);
    Device::setAccelerometerInterval(1 / 30);
    const auto listener = EventListenerAcceleration::create(CC_CALLBACK_2(PulseGameScene::onAccelerationDetected, this));
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void PulseGameScene::addCollisionListeners() {
    auto contactListener = EventListenerPhysicsContact::create();
    contactListener->onContactBegin = CC_CALLBACK_1(PulseGameScene::onContactBegan, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);
}

void PulseGameScene::addGameStateListeners() {
    gameState.onTimeModeChanged = [this](auto mode) {
        this->updateSceneTimeScale();
        if (mode == GameState::TimeMode::Normal) {
            this->handleDefeatedObstacles();
        }
    };
}

bool PulseGameScene::onTouchBegan(Touch* touch, Event* event) {
    if (sceneStatus() == GameScene::Status::Running) {
        gameState.enterMode(GameState::TimeMode::SlowMotion);
        return true;
    } else {
        resetScene();
        return false;
    }
}

void PulseGameScene::onTouchEnded(Touch* touch, Event* event) {
    if (sceneStatus() == GameScene::Status::Running) {
        gameState.enterMode(GameState::TimeMode::Normal);
    }
}

void PulseGameScene::onAccelerationDetected(Acceleration* acceleration, Event* event) {
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
    player->getPhysicsBody()->setVelocity(velocity * gameState.playerTimeScale());
}

bool PulseGameScene::onContactBegan(PhysicsContact &contact) {
    if (physics_body::collision::heroAndObstacle(contact)) {
        handleGameOver();
    } else if (physics_body::collision::heroAndPath(contact)) {
        const auto path = *physics_body::nodeInContact(contact, physics_body::isPath);
        const auto obstacle = static_cast<Obstacle*>(path->getParent());
        handlePassedObstacle(obstacle);
    }
    return false;
}

void PulseGameScene::handleGameOver() {
    gameState.gameOver();
    stopScene();
}

void PulseGameScene::handlePassedObstacle(Obstacle* obstacle) {
    gameState.incrementScore();
    updateScore();
    obstacle->getPhysicsBody()->pass();

    if (gameState.currentMode() == GameState::TimeMode::Normal) {
        handleDefeatedObstacle(obstacle);
    }
}

void PulseGameScene::handleDefeatedObstacle(Obstacle* obstacle) {
    obstacle->getPhysicsBody()->defeat();
    obstacle->runDefeatedActions();
}

void PulseGameScene::handleDefeatedObstacles() {
    const auto obstacleWasPassed = [](Obstacle* obstacle) {
        return obstacle->getPhysicsBody()->currentState() == ObstacleState::Passed;
    };
    const auto removeObstacle = [this](Obstacle* obstacle) {
        this->handleDefeatedObstacle(obstacle);
    };
    const auto passedObstacles = obstacles | ranges::view::filter(obstacleWasPassed) | ranges::to_vector;
    ranges::for_each(passedObstacles, removeObstacle);
}
