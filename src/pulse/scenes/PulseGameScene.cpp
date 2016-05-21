#include <string>
#include "range/v3/algorithm/any_of.hpp"
#include "range/v3/algorithm/for_each.hpp"

#include "pulse/scenes/PulseGameScene.hpp"
#include "pulse/actions/FollowedBy.hpp"
#include "pulse/actions/ObstacleSequence.hpp"
#include "pulse/sprites/SpritePhysicsBody.hpp"
#include "pulse/sprites/ObstaclePhysicsBody.hpp"
#include "pulse/generators/ObstacleGenerator.hpp"
#include "pulse/movement/AccelerometerMovementSystem.hpp"
#include "pulse/ui/Resources.hpp"
#include "pulse/utilities/Geometry.hpp"
#include "pulse/ui/Font.hpp"

using namespace cocos2d;
using namespace pulse;

PulseGameScene* PulseGameScene::create(GameOptions& options) {
    const auto scene = new (std::nothrow) PulseGameScene{options};
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
    addResetGameTouchListener();
    addTimeScaleTouchListener();
    addPlayerTouchListener();
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
    updateListeners(true);
    scheduleUpdate();
    scheduleObstacleGeneration();
    updateSceneTimeScale();
}

void PulseGameScene::stopScene() {
    GameScene::stopScene();
    updateListeners(false);
    unscheduleObstacleGeneration();
}

void PulseGameScene::updateListeners(bool isGameRunning) {
    resetListener->setEnabled(!isGameRunning);
    timeScaleListener->setEnabled(isGameRunning);
    playerTouchListener->setEnabled(isGameRunning);
}

void PulseGameScene::addBackground() {
    const auto particles = ParticleSystemQuad::create(Resources::Particles::AmbientBackground);
    particles->setPosition(sceneFrame().origin);
    particles->setSourcePosition(geometry::centerOf(sceneFrame()));
    particles->setPosVar(sceneFrame().size / 2);
    addChild(particles, -1);
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
    pauseItem->setPosition(Vec2{sceneFrame().origin.x + sceneFrame().size.width - 15, sceneFrame().origin.y + sceneFrame().size.height - 15});
    const auto menu = Menu::create(pauseItem, nullptr);
    menu->setPosition(Vec2::ZERO);
    addChild(menu, 3);
}

void PulseGameScene::addScoreLabel() {
    scoreLabel = Label::createWithTTF("", Font::System, 28);
    scoreLabel->setAnchorPoint(Vec2{0, 1.0});
    scoreLabel->setPosition(Vec2{sceneFrame().origin.x + 20, sceneFrame().origin.y + sceneFrame().size.height - 20});
    addChild(scoreLabel, 3);
}

void PulseGameScene::addPlayer() {
    player = WrappedSprite::create();
    const auto size = Size{30, 30};
    player->setContentSize(size);
    player->setTextureRect(Rect{Vec2::ZERO, size});
    player->setPosition(geometry::centerOf(sceneFrame()));
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
    const auto obstacleSequence = ObstacleSequence::create(obstacle, options.obstacleFrequency);
    const auto reschedule = CallFunc::create([this]() { scheduleObstacleGeneration(); });
    obstacleGenerator = Sequence::create(obstacleSequence, reschedule, nullptr);
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

void PulseGameScene::addResetGameTouchListener() {
    resetListener = EventListenerTouchOneByOne::create();
    resetListener->onTouchBegan = [this](auto touch, auto event) { return true; };
    resetListener->onTouchEnded = [this](auto touch, auto event) { this->resetScene(); };
    getEventDispatcher()->addEventListenerWithSceneGraphPriority(resetListener, this);
}

void PulseGameScene::addTimeScaleTouchListener() {
    timeScaleListener = EventListenerTouchOneByOne::create();
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
    playerTouchListener->onTouchBegan = [this](auto touch, auto event) {
        const auto touchEffect = ParticleSystemQuad::create(Resources::Particles::PulseBegan);
        player->runAction(FollowedBy::create(touchEffect));
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

bool PulseGameScene::onScreenCollision(const PhysicsContact& contact) const {
    return ranges::any_of(contact.getContactData()->points, [this](auto point) {
        return point != Vec2::ZERO && this->sceneFrame().containsPoint(point);
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
