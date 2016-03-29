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
    updateSceneTimeScale();
    addFlappy();
    scheduleUpdate();
    schedule(CC_SCHEDULE_SELECTOR(FlappyBirdScene::addObstacle), 1);
}

void FlappyBirdScene::clearScene() {
    GameScene::clearScene();

    stopAllActions();
    ranges::for_each(obstacles, [this](auto obstacle) { this->removeChild(obstacle); });
    removeChild(flappy);
    obstacles.clear();
    gameState.reset();
}

void FlappyBirdScene::addMenuOptions() {
    const auto pauseImage = Sprite::create();
    pauseImage->setColor(Color3B::GREEN);
    pauseImage->setContentSize(Size{40, 40});
    pauseImage->setTextureRect(pauseImage->getBoundingBox());
    const auto pauseItem = MenuItemSprite::create(pauseImage, pauseImage, CC_CALLBACK_1(FlappyBirdScene::onMenuPause, this));
    pauseItem->setAnchorPoint(Vec2{1.0, 1.0});
    pauseItem->setPosition(Vec2{frame.origin.x + frame.size.width - 15, frame.origin.y + frame.size.height - 15});
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

void addPhysicsBodyToObstacle(const Obstacle& obstacle) {
    obstacle.getTop()->setPhysicsBody(physics_body::createObstacle(obstacle.getTop()->getBoundingBox().size));
    obstacle.getBottom()->setPhysicsBody(physics_body::createObstacle(obstacle.getBottom()->getBoundingBox().size));
    obstacle.getGap()->setPhysicsBody(physics_body::createPath(obstacle.getGap()->getBoundingBox().size));
}

void FlappyBirdScene::addObstacle(float dt) {
    const auto onCompletion = [this](auto obstacle) { obstacles.remove(obstacle); };
    const auto obstacle = ObstacleGenerator{frame}.generate();
    addPhysicsBodyToObstacle(*obstacle);
    addChild(obstacle);
    obstacles.emplace_back(obstacle);
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
    contactListener->onContactSeparate = CC_CALLBACK_1(FlappyBirdScene::onContactEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);
}

bool FlappyBirdScene::onTouchBegan(Touch* touch, Event* event) {
    switch (sceneStatus()) {
        case GameScene::Status::Initialising:
            return false;
        case GameScene::Status::Running:
            gameState.enterMode(GameState::TimeMode::SlowMotion);
            updateSceneTimeScale();
            return true;
        case GameScene::Status::Paused:
            GameScene::resumeScene();
            return false;
        case GameScene::Status::Stopped:
            GameScene::resetScene();
            return false;
    }
}

void FlappyBirdScene::onTouchEnded(Touch* touch, Event* event) {
    switch (sceneStatus()) {
        case GameScene::Status::Running:
            gameState.enterMode(GameState::TimeMode::Normal);
            updateSceneTimeScale();
            break;
        default:
            break;
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
            const auto x = rotation::roll(currentAcceleration) - gameState.calibratedAccelerometerOffset().x;
            const auto y = rotation::pitch(currentAcceleration) - gameState.calibratedAccelerometerOffset().y;
            auto velocity = Vec2{x, y};
            velocity.scale(physics::AccelerometerScale);
            flappy->getPhysicsBody()->setVelocity(velocity * gameState.playerTimeScale());
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
        const auto obstacle = *physics_body::nodeInContact(contact, physics_body::isObstacle);
        handleObstacleCollision(static_cast<Obstacle*>(obstacle));
    } else if (physics_body::collision::heroAndPath(contact)) {
        const auto path = *physics_body::nodeInContact(contact, physics_body::isPath);
        const auto obstacle = static_cast<Obstacle*>(path->getParent());
        handlePassedObstacle(obstacle);
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

void FlappyBirdScene::handleObstacleCollision(Obstacle* obstacle) {
    GameScene::stopScene();
}

void FlappyBirdScene::handlePassedObstacle(Obstacle* obstacle) {
    gameState.incrementScore();
    updateScore();
    ranges::for_each(obstacle->getChildren(), [](const auto node) {
        physics_body::stopCollisions(node->getPhysicsBody());
    });
    obstacle->runDefeatedActions([this](auto obstacle) { obstacles.remove(obstacle); });
}
