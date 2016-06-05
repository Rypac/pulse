#include <string>
#include "range/v3/algorithm/for_each.hpp"

#include "pulse/scenes/PulseGameScene.hpp"
#include "pulse/actions/FollowedBy.hpp"
#include "pulse/actions/ObstacleSequence.hpp"
#include "pulse/extensions/Ref.hpp"
#include "pulse/sprites/SpritePhysicsBody.hpp"
#include "pulse/sprites/ObstaclePhysicsBody.hpp"
#include "pulse/generators/ObstacleGenerator.hpp"
#include "pulse/movement/AccelerometerMovementSystem.hpp"
#include "pulse/ui/Button.hpp"
#include "pulse/ui/Font.hpp"
#include "pulse/ui/Resources.hpp"
#include "pulse/extensions/Rect.hpp"

using namespace cocos2d;
using namespace pulse;

PulseGameScene::PulseGameScene(GameOptions& options): options{options}, gameState{GameState{options}} {
    getPhysicsWorld()->setGravity(Vec2::ZERO);
    getPhysicsWorld()->setAutoStep(false);

    addPlayer();
    addMenuOptions();
    addScoreLabel();
    addResetGameTouchListener();
    addTimeScaleTouchListener();
    addPlayerTouchListener();
    addPlayerMovementListener();
    addCollisionListeners();
    addGameStateListeners();
}

PulseGameScene::~PulseGameScene() {
    const auto listeners = {resetListener, timeScaleListener, playerTouchListener};
    ranges::for_each(listeners, [this](auto listener) {
        this->getEventDispatcher()->removeEventListener(listener);
        CC_SAFE_RELEASE(listener);
    });
    CC_SAFE_RELEASE(player);
    CC_SAFE_RELEASE(scoreLabel);
}

void PulseGameScene::onEnterTransitionDidFinish() {
    if (gameState.isGameOver()) {
        startScene();
    } else {
        resume();
    }
}

void PulseGameScene::resetScene() {
    stopScene();
    clearScene();
    gameState.gameOver();

    player->getPhysicsBody()->setVelocity(Vec2::ZERO);
    player->setPosition(rect::center(sceneFrame()));
}

void PulseGameScene::clearScene() {
    ranges::for_each(obstacles, [this](auto obstacle) {
        this->removeChild(obstacle);
    });
    obstacles.clear();
}

void PulseGameScene::startScene() {
    gameState.reset();

    updateScore();
    updateListeners(true);
    scheduleUpdate();
    scheduleObstacleGeneration();
    updateSceneTimeScale();
    resume();
}

void PulseGameScene::stopScene() {
    stopAllActions();
    player->stopAllActions();
    ranges::for_each(obstacles, [](auto obstacle) {
        obstacle->stopAllActions();
    });
    updateListeners(false);
    pause();
}

void PulseGameScene::updateListeners(bool isGameRunning) {
    resetListener->setEnabled(!isGameRunning);
    timeScaleListener->setEnabled(isGameRunning);
    playerTouchListener->setEnabled(isGameRunning);
}

void PulseGameScene::addMenuOptions() {
    const auto menuButton = ui::Button::create(Resources::Buttons::Pause);
    menuButton->setScale(0.6);
    menuButton->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
    menuButton->setPosition(Vec2{sceneFrame().getMaxX() - 20, sceneFrame().getMaxY() - 20});
    menuButton->onTouchEnded = [this](auto ref) {
        if (onEnterMenu) {
            onEnterMenu(this);
        }
    };
    addChild(menuButton, 3);
}

void PulseGameScene::addScoreLabel() {
    scoreLabel = Label::createWithTTF("", Font::System, 28);
    scoreLabel->retain();
    scoreLabel->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    scoreLabel->setPosition(Vec2{sceneFrame().getMinX() + 20, sceneFrame().getMaxY() - 20});
    addChild(scoreLabel, 3);
}

void PulseGameScene::addPlayer() {
    player = WrappedSprite::create();
    player->retain();
    const auto size = Size{30, 30};
    player->setContentSize(size);
    player->setTextureRect(Rect{Vec2::ZERO, size});
    player->setPosition(rect::center(sceneFrame()));
    player->setPhysicsBody(physics_body::createHero(player->getBoundingBox().size));
    addChild(player, 1);
}

Obstacle* PulseGameScene::generateObstacle() {
    const auto obstacle = ObstacleGenerator{sceneFrame()}.generate();
    obstacle->setSpeed(options.obstacleSpeed);
    obstacle->setPhysicsBody(ObstaclePhysicsBody::create(obstacle));
    obstacle->onStarted = [this](auto obstacle) { obstacles.emplace_back(obstacle); };
    obstacle->onCompletion = [this](auto obstacle) { obstacles.remove(obstacle); };
    return obstacle;
}

void PulseGameScene::scheduleObstacleGeneration() {
    const auto obstacle = generateObstacle();
    const auto obstacleSequence = autoreleased<ObstacleSequence>(obstacle, options.obstacleFrequency);
    const auto reschedule = CallFunc::create([this]() { scheduleObstacleGeneration(); });
    runAction(Sequence::create(obstacleSequence, reschedule, nullptr));
}

void PulseGameScene::update(float dt) {
    getPhysicsWorld()->step(dt);
}

void PulseGameScene::updateScore() {
    scoreLabel->setString("Score: " + std::to_string(gameState.currentScore()));
}

void PulseGameScene::updateSceneTimeScale() {
    getScheduler()->setTimeScale(gameState.environmentTimeScale());
}

void PulseGameScene::addResetGameTouchListener() {
    resetListener = EventListenerTouchOneByOne::create();
    resetListener->retain();
    resetListener->onTouchBegan = [this](auto touch, auto event) { return true; };
    resetListener->onTouchEnded = [this](auto touch, auto event) {
        this->resetScene();
        this->startScene();
    };
    getEventDispatcher()->addEventListenerWithFixedPriority(resetListener, 1);
}

void PulseGameScene::addTimeScaleTouchListener() {
    timeScaleListener = EventListenerTouchOneByOne::create();
    timeScaleListener->retain();
    timeScaleListener->onTouchBegan = [this](auto touch, auto event) {
        gameState.enterMode(GameState::TimeMode::SlowMotion);
        return true;
    };
    timeScaleListener->onTouchMoved = [this](auto touch, auto event) {
        if (touch->getMaxForce() > 0) {
            const auto maxForce = touch->getMaxForce();
            const auto currentForce = touch->getCurrentForce();
            options.slowMotionTimeScale.environment = ((maxForce - currentForce) / maxForce) + 0.1;
            this->updateSceneTimeScale();
        }
    };
    timeScaleListener->onTouchEnded = [this](auto touch, auto event) {
        gameState.enterMode(GameState::TimeMode::Normal);
    };
    getEventDispatcher()->addEventListenerWithSceneGraphPriority(timeScaleListener, this);
}

void PulseGameScene::addPlayerTouchListener() {
    playerTouchListener = EventListenerTouchOneByOne::create();
    playerTouchListener->retain();
    playerTouchListener->onTouchBegan = [this](auto touch, auto event) {
        const auto touchEffect = ParticleSystemQuad::create(Resources::Particles::PulseBegan);
        player->runAction(autoreleased<FollowedBy>(touchEffect));
        return true;
    };
    playerTouchListener->onTouchEnded = [this](auto touch, auto event) {
        player->stopAllActions();
        const auto touchEffect = ParticleSystemQuad::create(Resources::Particles::PulseEnded);
        touchEffect->setAutoRemoveOnFinish(true);
        touchEffect->setPosition(player->getPosition());
        this->addChild(touchEffect);
    };
    getEventDispatcher()->addEventListenerWithSceneGraphPriority(playerTouchListener, this);
}

void PulseGameScene::addPlayerMovementListener() {
    const auto accelerometerListener = AccelerometerMovementSystem::create(&gameState.accelerometer());
    accelerometerListener->onMovement = [this](const auto movedBy) {
        const auto velocity = Vec2{movedBy.x, movedBy.y} * gameState.playerTimeScale();
        player->getPhysicsBody()->setVelocity(velocity);
    };
    getEventDispatcher()->addEventListenerWithSceneGraphPriority(accelerometerListener, this);
}

void PulseGameScene::addCollisionListeners() {
    auto contactListener = EventListenerPhysicsContact::create();
    contactListener->onContactBegin = CC_CALLBACK_1(PulseGameScene::onContactBegan, this);
    contactListener->onContactPreSolve = CC_CALLBACK_2(PulseGameScene::onContactPreSolve, this);
    contactListener->onContactSeparate = CC_CALLBACK_1(PulseGameScene::onContactSeparate, this);
    getEventDispatcher()->addEventListenerWithSceneGraphPriority(contactListener, this);
}

void PulseGameScene::addGameStateListeners() {
    gameState.onTimeModeChanged = [this](auto mode) {
        this->updateSceneTimeScale();
    };
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
