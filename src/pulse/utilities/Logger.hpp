#pragma once

#include "cocos2d.h"

namespace pulse {
namespace logger {

void log(const cocos2d::Vec2& vec) {
    cocos2d::log("x = %f, y = %f", vec.x, vec.y);
}

void log(const cocos2d::Size& size) {
    cocos2d::log("width = %f, height = %f", size.width, size.height);
}

void log(const cocos2d::Rect& rect) {
    logger::log(rect.origin);
    logger::log(rect.size);
}

} }
