#pragma once

#include "cocos2d.h"

namespace flappy {
namespace log {

void vec(cocos2d::Vec2 vec) {
    cocos2d::log("x = %f, y = %f", vec.x, vec.y);
}

void size(cocos2d::Size size) {
    cocos2d::log("width = %f, height = %f", size.width, size.height);
}

void rect(cocos2d::Rect rect) {
    log::vec(rect.origin);
    log::size(rect.size);
}

} }
