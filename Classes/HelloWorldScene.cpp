#include "HelloWorldScene.h"
#include "geometry.hpp"

#include <gsl/gsl.h>
#include <range/v3/all.hpp>
#include <vector>

USING_NS_CC;

using geometry::centerOf;
using geometry::rightOf;
using gsl::owner;
using gsl::not_null;

owner<Scene*> HelloWorld::createScene() {
    auto scene = Scene::create();
    auto layer = HelloWorld::create();
    scene->addChild(layer);
    return scene;
}

owner<Sprite*> createFlappy() {
    auto flappy = Sprite::create();
    flappy->setTextureRect(Rect(0, 0, 30, 30));
    flappy->setColor(Color3B::WHITE);
    return flappy;
}

owner<Sprite*> createColumn(Rect sceneFrame) {
    auto spriteFrame = Rect(0, 0, 50, sceneFrame.size.height);
    auto column = Sprite::create();
    column->setAnchorPoint(Vec2(0, 0));
    column->setTextureRect(spriteFrame);
    column->setColor(Color3B::BLUE);
    return column;
}

owner<Sequence*> actionSequenceForColumn(not_null<Sprite*> column) {
    auto destination = Vec2(0 - column->getContentSize().width, 0);
    auto moveToEdge = MoveTo::create(5, destination);
    auto removeFromScene = RemoveSelf::create(true);
    return Sequence::create(moveToEdge, removeFromScene, nullptr);
}

void generateColumn(not_null<Layer*> scene, Rect frame) {
    auto column = createColumn(frame);
    column->setPosition(rightOf(column->getContentSize(), frame));
    scene->addChild(column);

    auto actions = actionSequenceForColumn(column);
    column->runAction(actions);
}

void HelloWorld::startColumnGenerator(Rect frame) {
    auto delay = DelayTime::create(2);
    auto generateNewColumn = CallFunc::create([this, frame]() { generateColumn(this, frame); });
    auto delayedColumnGenerator = Sequence::create(generateNewColumn, delay, nullptr);
    auto infiniteColumnGenerator = RepeatForever::create(delayedColumnGenerator);
    this->runAction(infiniteColumnGenerator);
}

bool HelloWorld::init() {
    if (!Layer::init()) {
        return false;
    }

    auto director = Director::getInstance();
    auto frame = Rect(director->getVisibleOrigin(), director->getVisibleSize());

    auto flappy = createFlappy();
    flappy->setPosition(centerOf(frame));
    this->addChild(flappy, 1);

    startColumnGenerator(frame);

    return true;
}
