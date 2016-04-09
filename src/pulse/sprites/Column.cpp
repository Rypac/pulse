#include "pulse/sprites/Column.hpp"

using namespace cocos2d;
using namespace pulse;

Column* Column::create() {
    Column *column = new (std::nothrow) Column();
    if (column && column->init()) {
        column->autorelease();
        return column;
    }
    delete column;
    return nullptr;
}

bool Column::init() {
    if (!Sprite::init()) {
        return false;
    }

    setColor(Color3B::BLUE);

    return true;
}

Column* Column::create(float height) {
    return create(Rect{0, 0, defaultLength, height});
}

Column* Column::create(Rect frame) {
    const auto column = Column::create();
    column->setPosition(frame.origin);
    column->setContentSize(frame.size);
    column->setTextureRect(frame);
    return column;
}
