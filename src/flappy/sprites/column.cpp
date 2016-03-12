#include <flappy/sprites/column.hpp>

USING_NS_CC;
using namespace flappy;

bool Column::init() {
    if (!Sprite::init()) {
        return false;
    }

    setAnchorPoint(Vec2(0, 0));
    setColor(Color3B::BLUE);

    return true;
}

Column* Column::create(Rect sceneFrame) {
    auto columnFrame = Rect(0, 0, 50, sceneFrame.size.height);
    auto column = Column::create();
    column->setTextureRect(columnFrame);
    return column;
}

Sequence* Column::actionSequence() {
    auto destination = Vec2(0 - getContentSize().width, 0);
    auto moveToEdge = MoveTo::create(5, destination);
    auto removeFromScene = RemoveSelf::create(true);
    return Sequence::create(moveToEdge, removeFromScene, nullptr);
}
