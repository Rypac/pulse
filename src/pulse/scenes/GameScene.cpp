#include <string>

#include "pulse/actions/CallbackAfter.hpp"
#include "pulse/actions/FollowedBy.hpp"
#include "pulse/actions/ObstacleSequence.hpp"
#include "pulse/extensions/Node.hpp"
#include "pulse/extensions/Rect.hpp"
#include "pulse/extensions/Ref.hpp"
#include "pulse/generators/ObstacleGenerator.hpp"
#include "pulse/movement/AccelerometerMovementSystem.hpp"
#include "pulse/scenes/GameScene.hpp"
#include "pulse/sprites/ObstaclePhysicsBody.hpp"
#include "pulse/sprites/SpritePhysicsBody.hpp"
#include "pulse/ui/Button.hpp"
#include "pulse/ui/Colour.hpp"
#include "pulse/ui/Font.hpp"
#include "pulse/ui/Resources.hpp"
#include "pulse/utilities/Callback.hpp"

using namespace cocos2d;
using namespace pulse;

GameScene::GameScene(const GameOptions& options)
: gameState{GameState{options}} {
    getPhysicsWorld()->setGravity(Vec2::ZERO);

    addPlayer();
    addMenuOptions();
    addScoreLabel();
    addPowerupMeter();
    addGameStateListeners();

    gameListeners = {
        timeScaleTouchListener(),
        playerMovementListener(),
        collisionListener(),
    };
    for (auto&& listener : gameListeners) {
        getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
    }
}

GameScene::~GameScene() {
    for (auto&& listener : gameListeners) {
        getEventDispatcher()->removeEventListener(listener);
        CC_SAFE_RELEASE(listener);
    }
    CC_SAFE_RELEASE(player);
}

void GameScene::startNewGame() {
    gameState.newGame();
}

void GameScene::startScene() {
    updateListeners(true);
    scheduleUpdate();
    scheduleObstacleGeneration();
}

void GameScene::stopScene() {
    node::stopAllActionsRecursively(this);
    player->setVelocity(Vec2::ZERO);
    updateListeners(false);
    getScheduler()->setTimeScale(1.0);
}

void GameScene::resetScene() {
    for (auto&& obstacle : obstacles) {
        removeChild(obstacle);
    }
    obstacles.clear();
    player->setPosition(rect::center(sceneFrame()));
}

void GameScene::updateListeners(bool isGameRunning) {
    for (auto&& listener : gameListeners) {
        listener->setEnabled(isGameRunning);
    }
}

void GameScene::addMenuOptions() {
    const auto menuButton = ui::Button::create(Resources::Buttons::Pause);
    menuButton->setScale(0.6);
    menuButton->setPosition(sceneFrame().getMaxX() - 50, sceneFrame().getMaxY() - 50);
    menuButton->setPhysicsBody(physics_body::createUI(menuButton->getBoundingBox().size));
    menuButton->onTouchEnded = [this](auto ref) {
        safe_callback(onEnterMenu, this);
    };
    addChild(menuButton, 3);
}

void GameScene::addPowerupMeter() {
    powerupMeter = Sprite::create();
    powerupMeter->setColor(Colour::Blue);
    const auto origin = Vec2{sceneFrame().getMidX(), sceneFrame().getMaxY() - 45};
    const auto size = Size{sceneFrame().size.width * 0.75f, 20};
    const auto frame = Rect{origin, size};
    powerupMeter->setPosition(frame.origin);
    powerupMeter->setContentSize(frame.size);
    powerupMeter->setTextureRect(frame);
    powerupMeter->setPhysicsBody(physics_body::createUI(frame.size));
    addChild(powerupMeter, 3);
}

void GameScene::addScoreLabel() {
    score = Score::create(0);
    score->setCascadeOpacityEnabled(true);
    score->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    score->setScale(0.2);
    score->setPosition(sceneFrame().getMinX() + 10, sceneFrame().getMaxY() - 10);
    score->setPhysicsBody(physics_body::createUI(score->getBoundingBox().size));
    addChild(score, 3);
}

void GameScene::addPlayer() {
    player = Player::create();
    player->retain();
    const auto size = Size{30, 30};
    player->setContentSize(size);
    player->setTextureRect(Rect{Vec2::ZERO, size});
    player->setPosition(rect::center(sceneFrame()));
    player->setPhysicsBody(physics_body::createHero(player->getBoundingBox().size));
    addChild(player, 1);
}

Obstacle* GameScene::generateObstacle() {
    const auto obstacle = ObstacleGenerator{sceneFrame()}.generate();
    obstacle->setSpeed(gameState.obstacleSpeed());
    obstacle->setPhysicsBody(autoreleased<ObstaclePhysicsBody>(obstacle));
    obstacle->onStarted = [this](auto obstacle) {
        obstacles.emplace_back(obstacle);
    };
    obstacle->onCompletion = [this](auto obstacle) {
        obstacles.remove(obstacle);
    };
    return obstacle;
}

void GameScene::scheduleObstacleGeneration() {
    const auto obstacle = generateObstacle();
    const auto obstacleSequence = autoreleased<ObstacleSequence>(obstacle, gameState.obstacleFrequency());
    const auto reschedule = [this]() {
        this->scheduleObstacleGeneration();
    };
    runAction(autoreleased<CallbackAfter>(obstacleSequence, reschedule));
}

cocos2d::EventListener* GameScene::timeScaleTouchListener() {
    const auto timeScaleListener = EventListenerTouchOneByOne::create();
    timeScaleListener->retain();
    timeScaleListener->onTouchBegan = [this](auto touch, auto event) {
        if (not gameState.powerup().isActive()) {
            return false;
        }
        gameState.enterMode(GameState::TimeMode::SlowMotion);
        this->playSlowdownAnimation();
        return true;
    };
    timeScaleListener->onTouchEnded = [this](auto touch, auto event) {
        gameState.enterMode(GameState::TimeMode::Normal);
        this->playSpeedupAnimation();
    };
    return timeScaleListener;
}

cocos2d::EventListener* GameScene::playerMovementListener() {
    const auto movementListener = retained<AccelerometerMovementSystem>(&gameState.accelerometer());
    movementListener->onMovement = [this](const auto movedBy) {
        const auto velocity = Vec2{movedBy.x, movedBy.y} * gameState.playerTimeScale();
        player->setVelocity(velocity);
    };
    return movementListener;
}

cocos2d::EventListener* GameScene::collisionListener() {
    auto contactListener = EventListenerPhysicsContact::create();
    contactListener->retain();
    contactListener->onContactBegin = [this](auto& contact) {
        if (this->isObstacleCollision(contact)) {
            gameState.gameOver();
        }
        if (physics_body::collision::withUI(contact)) {
            const auto ui = *physics_body::nodeInContact(contact, physics_body::isUI);
            ui->setOpacity(160);
        }
        return not physics_body::collision::heroAndPath(contact);
    };
    contactListener->onContactPreSolve = [this](auto& contact, auto& solve) {
        if (this->isObstacleCollision(contact)) {
            gameState.gameOver();
        }
        return false;
    };
    contactListener->onContactSeparate = [this](auto& contact) {
        if (this->isPathCollision(contact)) {
            const auto path = *physics_body::nodeInContact(contact, physics_body::isPath);
            const auto obstacle = static_cast<Obstacle*>(path->getParent());
            obstacle->getPhysicsBody()->defeat();
            obstacle->runDefeatedActions();
            gameState.incrementScore();
        }
        if (physics_body::collision::withUI(contact)) {
            const auto ui = *physics_body::nodeInContact(contact, physics_body::isUI);
            ui->setOpacity(255);
        }
    };
    return contactListener;
}

void GameScene::addGameStateListeners() {
    gameState.onNewGame = [this]() {
        this->stopScene();
        this->resetScene();
        this->startScene();
    };
    gameState.onGameOver = [this]() {
        this->stopScene();
        this->runAction(autoreleased<CallbackAfterDelay>(0.5, [this]() {
            safe_callback(onGameOver, this, gameState.currentScore());
        }));
        player->setColor(Colour::Blue);
    };
    gameState.onScoreChanged = [this]() {
        score->setScore(gameState.currentScore());
    };
    gameState.onTimeModeChanged = [this](auto mode) {
        this->getScheduler()->setTimeScale(gameState.environmentTimeScale());
        if (mode == GameState::TimeMode::SlowMotion) {
            this->startPowerupTimer();
        } else {
            this->unschedule("PowerupTimer");
        }
    };
}

void GameScene::startPowerupTimer() {
    schedule(
        [this](auto dt) {
            gameState.powerup().elapse(dt);
            if (not gameState.powerup().isActive()) {
                powerupMeter->setVisible(false);
                gameState.enterMode(GameState::TimeMode::Normal);
            } else {
                powerupMeter->setScaleX(gameState.powerup().remainingTime() / 0.8f);
            }
        },
        "PowerupTimer");
}

void GameScene::playSpeedupAnimation() {
    player->stopAllActions();
    const auto touchEffect = ParticleSystemQuad::create(Resources::Particles::PulseEnded);
    touchEffect->setAutoRemoveOnFinish(true);
    touchEffect->setPosition(player->getPosition());
    this->addChild(touchEffect);
}

void GameScene::playSlowdownAnimation() {
    const auto touchEffect = ParticleSystemQuad::create(Resources::Particles::PulseBegan);
    player->runAction(autoreleased<FollowedBy>(touchEffect));
}

bool GameScene::isObstacleCollision(const cocos2d::PhysicsContact& contact) const {
    return physics_body::collision::heroAndObstacle(contact) and onScreenCollision(contact);
}

bool GameScene::isPathCollision(const cocos2d::PhysicsContact& contact) const {
    return physics_body::collision::heroAndPath(contact);
}
