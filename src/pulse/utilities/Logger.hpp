#pragma once

#include "cocos2d.h"

namespace pulse {
namespace log {

void vec(const cocos2d::Vec2& vec) {
    cocos2d::log("x = %f, y = %f", vec.x, vec.y);
}

void size(const cocos2d::Size& size) {
    cocos2d::log("width = %f, height = %f", size.width, size.height);
}

void rect(const cocos2d::Rect& rect) {
    log::vec(rect.origin);
    log::size(rect.size);
}

} }
