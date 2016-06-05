#include "pulse/sprites/Column.hpp"

using namespace cocos2d;
using pulse::Column;

Column::Column(): Column(0) {}

Column::Column(float height): Column(Rect{0, 0, defaultLength, height}) {}

Column::Column(const Rect& frame) {
    init();
    setColor(Color3B::BLUE);
    setPosition(frame.origin);
    setContentSize(frame.size);
    setTextureRect(frame);
}
