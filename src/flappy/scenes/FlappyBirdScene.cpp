#include "flappy/scenes/FlappyBirdScene.hpp"
#include "flappy/sprites/Column.hpp"
#include "flappy/sprites/FlappyBird.hpp"
#include "flappy/sprites/Obstacle.hpp"
#include "flappy/utilities/Geometry.hpp"
#include "flappy/utilities/Physics.hpp"
#include "flappy/utilities/Random.hpp"

using namespace cocos2d;
using namespace flappy;

Scene* GameScene::createScene() {
    auto scene = Scene::create();
    auto layer = FlappyBirdScene::create();
    scene->addChild(layer);
    return scene;
}

bool FlappyBirdScene::init() {
    if (!GameScene::init()) {
        return false;
    }

    addFlappy();
    generateObstacles();
    addTouchListeners();
    addKeyboardListeners();
    scheduleUpdate();

    return true;
}

void FlappyBirdScene::addFlappy() {
    flappy = FlappyBird::create();
    flappy->setPosition(geometry::centerOf(frame));
    addChild(flappy, 1);
}

Obstacle FlappyBirdScene::generateObstacle() {
    const auto bottomHeight = random::between(0, frame.size.height - Column::gapHeight);
    const auto topHeight = frame.size.height - Column::gapHeight - bottomHeight;

    const auto obstacle = Obstacle(Column::create(topHeight), Column::create(bottomHeight));
    obstacle.bottom->setPosition(Vec2(frame.size.width + obstacle.bottom->getBoundingBox().size.width, 0));
    obstacle.top->setPosition(Vec2(frame.size.width + obstacle.top->getBoundingBox().size.width, bottomHeight + Column::gapHeight));
    return obstacle;
}

void FlappyBirdScene::addObstacle() {
    auto obstacle = generateObstacle();
    obstacle.runActions();
    obstacle.onActionsCompleted = [this](const auto& obstacle) { passedObstacles.pop_back(); };
    addChild(obstacle.bottom);
    addChild(obstacle.top);
    incomingObstacles.emplace_back(obstacle);
}

void FlappyBirdScene::generateObstacles() {
    auto delay = DelayTime::create(2);
    auto generateNewColumn = CallFunc::create([this]() { this->addObstacle(); });
    auto delayedColumnGenerator = Sequence::create(generateNewColumn, delay, nullptr);
    auto infiniteColumnGenerator = RepeatForever::create(delayedColumnGenerator);
    runAction(infiniteColumnGenerator);
}

std::optional<Obstacle> FlappyBirdScene::nearestObstacle() {
    return incomingObstacles.empty() ? std::nullopt : std::make_optional(incomingObstacles.front());
}

void FlappyBirdScene::update(float dt) {
    flappy->update(dt);

    auto possibleNearestObstacle = nearestObstacle();
    if (!possibleNearestObstacle) {
        return;
    }

    auto obstacle = *possibleNearestObstacle;
    auto flappyFrame = flappy->getBoundingBox();
    if (obstacle.collidesWith(flappyFrame)) {
        log("Hit!");
        score = 0;
        GameScene::pauseScene();
    } else if (obstacle.passedBy(flappyFrame)) {
        ++score;
        log("Score: %d", score);
        incomingObstacles.pop_front();
        passedObstacles.emplace_back(obstacle);
    }
}

void FlappyBirdScene::addTouchListeners() {
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = CC_CALLBACK_2(FlappyBirdScene::onTouchBegan, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void FlappyBirdScene::addKeyboardListeners() {
    auto listener = EventListenerKeyboard::create();
    listener->onKeyPressed = [this](auto keycode, auto event) {
        if (keycode == EventKeyboard::KeyCode::KEY_SPACE) {
            GameScene::isScenePaused() ? GameScene::resumeScene() : GameScene::pauseScene();
        }
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

bool FlappyBirdScene::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event) {
    flappy->velocity.y = 850;
    return true;
}
