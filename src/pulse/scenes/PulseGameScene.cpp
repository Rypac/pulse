#include <string>
#include "range/v3/algorithm/any_of.hpp"
#include "range/v3/algorithm/for_each.hpp"

#include "pulse/scenes/PulseGameScene.hpp"
#include "pulse/sprites/SpritePhysicsBody.hpp"
#include "pulse/sprites/ObstaclePhysicsBody.hpp"
#include "pulse/generators/ObstacleGenerator.hpp"
#include "pulse/movement/AccelerometerMovementSystem.hpp"
#include "pulse/utilities/Geometry.hpp"
#include "pulse/ui/Font.hpp"

using namespace cocos2d;
using namespace pulse;

PulseGameScene* PulseGameScene::create(GameOptions& options) {
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

    addBackground();
    addMenuOptions();
    addScoreLabel();
    addTouchListeners();
    addPlayerMovementListener();
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

void PulseGameScene::addBackground() {
    const auto background = Sprite::create("background/level.png");
    background->setPosition(geometry::centerOf(frame));
    addChild(background, -1);
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
    scoreLabel = Label::createWithTTF("", Font::System, 28);
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
    addChild(obstacle, 2);
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
        obstacleGenerator = nullptr;
    }
}

void PulseGameScene::update(float dt) {
    getScene()->getPhysicsWorld()->step(dt);
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
    listener->onTouchMoved = CC_CALLBACK_2(PulseGameScene::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(PulseGameScene::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void PulseGameScene::addPlayerMovementListener() {
    const auto playerMovement = AccelerometerMovementSystem::create(&gameState.accelerometer());
    playerMovement->onMovement = [this](const auto movedBy) {
        const auto velocity = Vec2{movedBy.x, movedBy.y} * gameState.playerTimeScale();
        player->getPhysicsBody()->setVelocity(velocity);
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(playerMovement, this);
}

void PulseGameScene::addCollisionListeners() {
    auto contactListener = EventListenerPhysicsContact::create();
    contactListener->onContactBegin = CC_CALLBACK_1(PulseGameScene::onContactBegan, this);
    contactListener->onContactPreSolve = CC_CALLBACK_2(PulseGameScene::onContactPreSolve, this);
    contactListener->onContactSeparate = CC_CALLBACK_1(PulseGameScene::onContactSeparate, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);
}

void PulseGameScene::addGameStateListeners() {
    gameState.onTimeModeChanged = [this](auto mode) {
        this->updateSceneTimeScale();
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

void PulseGameScene::onTouchMoved(Touch* touch, Event* event) {
    if (sceneStatus() == GameScene::Status::Running) {
        if (touch->getMaxForce() > 0) {
            on3dTouchDetected(touch);
        }
    }
}

void PulseGameScene::on3dTouchDetected(Touch* touch) {
    const auto maxForce = touch->getMaxForce();
    const auto currentForce = touch->getCurrentForce();
    options.slowMotionTimeScale.environment = ((maxForce - currentForce) / maxForce) + 0.1;
    updateSceneTimeScale();
}

void PulseGameScene::onTouchEnded(Touch* touch, Event* event) {
    if (sceneStatus() == GameScene::Status::Running) {
        gameState.enterMode(GameState::TimeMode::Normal);
    }
}

bool PulseGameScene::onScreenCollision(const PhysicsContact &contact) const {
    return ranges::any_of(contact.getContactData()->points, [this](auto point) {
        return point != Vec2::ZERO && frame.containsPoint(point);
    });
}

bool PulseGameScene::onContactBegan(PhysicsContact& contact) {
    return !physics_body::collision::heroAndPath(contact);
}

bool PulseGameScene::onContactPreSolve(PhysicsContact& contact, PhysicsContactPreSolve& solve) {
    if (physics_body::collision::heroAndObstacle(contact) && onScreenCollision(contact)) {
        handleGameOver();
    }
    return false;
}

void PulseGameScene::onContactSeparate(PhysicsContact& contact) {
    if (physics_body::collision::heroAndPath(contact)) {
        const auto path = *physics_body::nodeInContact(contact, physics_body::isPath);
        const auto obstacle = static_cast<Obstacle*>(path->getParent());
        handlePassedObstacle(obstacle);
    }
}

void PulseGameScene::handleGameOver() {
    gameState.gameOver();
    stopScene();
}

void PulseGameScene::handlePassedObstacle(Obstacle* obstacle) {
    gameState.incrementScore();
    updateScore();
    obstacle->getPhysicsBody()->defeat();
    obstacle->runDefeatedActions();
}
