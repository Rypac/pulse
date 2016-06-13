#include <string>

#include "pulse/scenes/PulseGameScene.hpp"
#include "pulse/actions/CallbackAfter.hpp"
#include "pulse/actions/FollowedBy.hpp"
#include "pulse/actions/ObstacleSequence.hpp"
#include "pulse/extensions/Rect.hpp"
#include "pulse/extensions/Ref.hpp"
#include "pulse/sprites/SpritePhysicsBody.hpp"
#include "pulse/sprites/ObstaclePhysicsBody.hpp"
#include "pulse/generators/ObstacleGenerator.hpp"
#include "pulse/movement/AccelerometerMovementSystem.hpp"
#include "pulse/ui/Button.hpp"
#include "pulse/ui/Font.hpp"
#include "pulse/ui/Resources.hpp"
#include "pulse/utilities/Callback.hpp"

using namespace cocos2d;
using namespace pulse;

PulseGameScene::PulseGameScene(const GameOptions& options): gameState{GameState{options}} {
    getPhysicsWorld()->setGravity(Vec2::ZERO);

    addPlayer();
    addMenuOptions();
    addScoreLabel();
    addResetGameTouchListener();
    addGameStateListeners();

    gameListeners = {
        timeScaleTouchListener(),
        playerTouchListener(),
        playerMovementListener(),
        collisionListener()
    };
    for (auto listener : gameListeners) {
        getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
    }

    setonEnterTransitionDidFinishCallback([this]() {
        if (not gameState.hasGameStarted()) {
            this->startNewGame();
        }
    });
}

PulseGameScene::~PulseGameScene() {
    for (auto listener : gameListeners) {
        getEventDispatcher()->removeEventListener(listener);
        CC_SAFE_RELEASE(listener);
    }
    getEventDispatcher()->removeEventListener(resetListener);
    CC_SAFE_RELEASE(resetListener);
    CC_SAFE_RELEASE(player);
    CC_SAFE_RELEASE(scoreLabel);
}

void PulseGameScene::startNewGame() {
    gameState.newGame();
}

void PulseGameScene::startScene() {
    updateScore();
    getEventDispatcher()->resumeEventListenersForTarget(this);
    resetListener->setEnabled(false);
    updateListeners(true);
    scheduleUpdate();
    scheduleObstacleGeneration();
    updateSceneTimeScale();
}

void PulseGameScene::stopScene() {
    stopAllActions();
    player->stopAllActions();
    player->setVelocity(Vec2::ZERO);
    for (auto obstacle : obstacles) {
        obstacle->stopAllActions();
    }
    updateListeners(false);
}

void PulseGameScene::resetScene() {
    for (auto obstacle : obstacles) {
        removeChild(obstacle);
    }
    obstacles.clear();
    player->setPosition(rect::center(sceneFrame()));
}

void PulseGameScene::updateListeners(bool isGameRunning) {
    resetListener->setEnabled(!isGameRunning);
    for (auto listener : gameListeners) {
        listener->setEnabled(isGameRunning);
    }
}

void PulseGameScene::addMenuOptions() {
    const auto menuButton = ui::Button::create(Resources::Buttons::Pause);
    menuButton->setScale(0.6);
    menuButton->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
    menuButton->setPosition(Vec2{sceneFrame().getMaxX() - 20, sceneFrame().getMaxY() - 20});
    menuButton->onTouchEnded = [this](auto ref) {
        safe_callback(onEnterMenu, this);
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
    player = Player::create();
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
    obstacle->setSpeed(gameState.obstacleSpeed());
    obstacle->setPhysicsBody(autoreleased<ObstaclePhysicsBody>(obstacle));
    obstacle->onStarted = [this](auto obstacle) { obstacles.emplace_back(obstacle); };
    obstacle->onCompletion = [this](auto obstacle) { obstacles.remove(obstacle); };
    return obstacle;
}

void PulseGameScene::scheduleObstacleGeneration() {
    const auto obstacle = generateObstacle();
    const auto obstacleSequence = autoreleased<ObstacleSequence>(obstacle, gameState.obstacleFrequency());
    const auto reschedule = [this]() { this->scheduleObstacleGeneration(); };
    runAction(autoreleased<CallbackAfter>(obstacleSequence, reschedule));
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
        gameState.newGame();
    };
    getEventDispatcher()->addEventListenerWithFixedPriority(resetListener, 1);
}

cocos2d::EventListener* PulseGameScene::timeScaleTouchListener() {
    const auto timeScaleListener = EventListenerTouchOneByOne::create();
    timeScaleListener->retain();
    timeScaleListener->onTouchBegan = [this](auto touch, auto event) {
        gameState.enterMode(GameState::TimeMode::SlowMotion);
        return true;
    };
    timeScaleListener->onTouchEnded = [this](auto touch, auto event) {
        gameState.enterMode(GameState::TimeMode::Normal);
    };
    return timeScaleListener;
}

cocos2d::EventListener* PulseGameScene::playerTouchListener() {
    const auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->retain();
    touchListener->onTouchBegan = [this](auto touch, auto event) {
        const auto touchEffect = ParticleSystemQuad::create(Resources::Particles::PulseBegan);
        player->runAction(autoreleased<FollowedBy>(touchEffect));
        return true;
    };
    touchListener->onTouchEnded = [this](auto touch, auto event) {
        player->stopAllActions();
        const auto touchEffect = ParticleSystemQuad::create(Resources::Particles::PulseEnded);
        touchEffect->setAutoRemoveOnFinish(true);
        touchEffect->setPosition(player->getPosition());
        this->addChild(touchEffect);
    };
    return touchListener;
}

cocos2d::EventListener* PulseGameScene::playerMovementListener() {
    const auto movementListener = retained<AccelerometerMovementSystem>(&gameState.accelerometer());
    movementListener->onMovement = [this](const auto movedBy) {
        const auto velocity = Vec2{movedBy.x, movedBy.y} * gameState.playerTimeScale();
        player->setVelocity(velocity);
    };
    return movementListener;
}

cocos2d::EventListener* PulseGameScene::collisionListener() {
    auto contactListener = EventListenerPhysicsContact::create();
    contactListener->retain();
    contactListener->onContactBegin = CC_CALLBACK_1(PulseGameScene::onContactBegan, this);
    contactListener->onContactPreSolve = CC_CALLBACK_2(PulseGameScene::onContactPreSolve, this);
    contactListener->onContactSeparate = CC_CALLBACK_1(PulseGameScene::onContactSeparate, this);
    return contactListener;
}

void PulseGameScene::addGameStateListeners() {
    gameState.onNewGame = [this]() {
        this->resetScene();
        this->startScene();
    };
    gameState.onGameOver = [this]() {
        this->stopScene();
    };
    gameState.onTimeModeChanged = [this](auto mode) {
        this->updateSceneTimeScale();
    };
}

bool PulseGameScene::onContactBegan(PhysicsContact& contact) {
    return !physics_body::collision::heroAndPath(contact);
}

bool PulseGameScene::onContactPreSolve(PhysicsContact& contact, PhysicsContactPreSolve& solve) {
    if (physics_body::collision::heroAndObstacle(contact) && onScreenCollision(contact)) {
        gameState.gameOver();
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

void PulseGameScene::handlePassedObstacle(Obstacle* obstacle) {
    gameState.incrementScore();
    updateScore();
    obstacle->getPhysicsBody()->defeat();
    obstacle->runDefeatedActions();
}
