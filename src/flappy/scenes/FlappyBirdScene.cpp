#include <flappy/scenes/FlappyBirdScene.hpp>
#include <flappy/sprites/column.hpp>
#include <flappy/utilities/geometry.hpp>

using namespace cocos2d;
using namespace flappy;
using namespace geometry;

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
    generateColumns();

    return true;
}

void FlappyBirdScene::addFlappy() {
    auto flappy = Sprite::create();
    flappy->setTextureRect(Rect(0, 0, 30, 30));
    flappy->setColor(Color3B::WHITE);
    flappy->setPosition(centerOf(frame));
    addChild(flappy, 1);
}

void FlappyBirdScene::addColumn() {
    auto column = Column::create(frame);
    column->setPosition(rightOf(column->getContentSize(), frame));
    column->runAction(column->actionSequence());
    addChild(column);
}

void FlappyBirdScene::generateColumns() {
    auto delay = DelayTime::create(2);
    auto generateNewColumn = CallFunc::create([this]() { this->addColumn(); });
    auto delayedColumnGenerator = Sequence::create(generateNewColumn, delay, nullptr);
    auto infiniteColumnGenerator = RepeatForever::create(delayedColumnGenerator);
    this->runAction(infiniteColumnGenerator);
}
