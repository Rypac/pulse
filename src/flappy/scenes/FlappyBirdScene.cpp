#include <flappy/scenes/FlappyBirdScene.hpp>
#include <flappy/sprites/Column.hpp>
#include <flappy/sprites/FlappyBird.hpp>
#include <flappy/utilities/Geometry.hpp>
#include <flappy/utilities/Random.hpp>

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
    generateColumns();

    return true;
}

void FlappyBirdScene::addFlappy() {
    flappy = FlappyBird::create();
    flappy->setPosition(geometry::centerOf(frame));
    addChild(flappy, 1);
}

void FlappyBirdScene::addColumn() {
    const auto columnStartX = frame.size.width + Column::defaultWidth;
    const auto bottomHeight = random::between(0, frame.size.height - Column::gapHeight);
    const auto topHeight = frame.size.height - Column::gapHeight - bottomHeight;

    auto bottomColumn = Column::create(bottomHeight);
    auto topColumn = Column::create(topHeight);

    bottomColumn->setPosition(Vec2(columnStartX, 0));
    topColumn->setPosition(Vec2(columnStartX, bottomHeight + Column::gapHeight));

    const auto bottomColumnActions = bottomColumn->actionSequence();
    const auto topColumnActions = topColumn->actionSequence();
    bottomColumn->runAction(bottomColumnActions);
    topColumn->runAction(topColumnActions);
    addChild(bottomColumn);
    addChild(topColumn);
}

void FlappyBirdScene::generateColumns() {
    auto delay = DelayTime::create(2);
    auto generateNewColumn = CallFunc::create([this]() { this->addColumn(); });
    auto delayedColumnGenerator = Sequence::create(generateNewColumn, delay, nullptr);
    auto infiniteColumnGenerator = RepeatForever::create(delayedColumnGenerator);
    runAction(infiniteColumnGenerator);
}
