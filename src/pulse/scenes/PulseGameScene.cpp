#include <string>

#include "pulse/scenes/PulseGameScene.hpp"
#include "pulse/actions/CallbackAfter.hpp"
#include "pulse/actions/FollowedBy.hpp"
#include "pulse/actions/ObstacleSequence.hpp"
#include "pulse/extensions/Node.hpp"
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
}

PulseGameScene::~PulseGameScene() {
    for (auto listener : gameListeners) {
        getEventDispatcher()->removeEventListener(listener);
        CC_SAFE_RELEASE(listener);
    }
    CC_SAFE_RELEASE(player);
    CC_SAFE_RELEASE(scoreLabel);
}

void PulseGameScene::startNewGame() {
    gameState.newGame();
}

void PulseGameScene::startScene() {
    updateListeners(true);
    scheduleUpdate();
    scheduleObstacleGeneration();
}

void PulseGameScene::stopScene() {
    node::stopAllActionsRecursively(this);
    player->setVelocity(Vec2::ZERO);
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
    contactListener->onContactBegin = [this](auto& contact) {
        this->checkForObstacleCollision(contact);
        return !physics_body::collision::heroAndPath(contact);
    };
    contactListener->onContactPreSolve = [this](auto& contact, auto& solve) {
        this->checkForObstacleCollision(contact);
        return false;
    };
    contactListener->onContactSeparate = [this](auto& contact) {
        if (physics_body::collision::heroAndPath(contact)) {
            const auto path = *physics_body::nodeInContact(contact, physics_body::isPath);
            const auto obstacle = static_cast<Obstacle*>(path->getParent());
            this->handlePassedObstacle(obstacle);
        }
    };
    return contactListener;
}

void PulseGameScene::addGameStateListeners() {
    gameState.onNewGame = [this]() {
        this->stopScene();
        this->resetScene();
        this->startScene();
    };
    gameState.onGameOver = [this]() {
        this->stopScene();
        safe_callback(onGameOver, this);
    };
    gameState.onScoreChanged = [this]() {
        scoreLabel->setString("Score: " + std::to_string(gameState.currentScore()));
    };
    gameState.onTimeModeChanged = [this](auto mode) {
        this->getScheduler()->setTimeScale(gameState.environmentTimeScale());
    };
}

void PulseGameScene::checkForObstacleCollision(const cocos2d::PhysicsContact& contact) {
    if (physics_body::collision::heroAndObstacle(contact) && onScreenCollision(contact)) {
        gameState.gameOver();
    }
}

void PulseGameScene::handlePassedObstacle(Obstacle* obstacle) {
    gameState.incrementScore();
    obstacle->getPhysicsBody()->defeat();
    obstacle->runDefeatedActions();
}
