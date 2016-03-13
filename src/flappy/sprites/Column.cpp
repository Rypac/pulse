#include "flappy/sprites/Column.hpp"

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

Column* Column::create(float height) {
    return create(Rect(0, 0, defaultWidth, height));
}

Column* Column::create(Rect frame) {
    auto column = Column::create();
    column->setTextureRect(frame);
    return column;
}
