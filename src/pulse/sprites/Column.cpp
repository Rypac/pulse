#include "pulse/sprites/Column.hpp"
#include "pulse/ui/Colour.hpp"

using cocos2d::Rect;
using pulse::Column;

Column::Column(): Column(0) {}

Column::Column(float height): Column(Rect{0, 0, defaultLength, height}) {}

Column::Column(const Rect& frame) {
    init();
    setColor(Colour::Orange);
    setPosition(frame.origin);
    setContentSize(frame.size);
    setTextureRect(frame);
}
