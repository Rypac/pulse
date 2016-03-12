#pragma once

#include "cocos2d.h"

namespace flappy { namespace geometry {

using cocos2d::Rect;
using cocos2d::Size;
using cocos2d::Vec2;

static inline Vec2 centerOf(Rect frame) {
    return Vec2(frame.origin.x + frame.size.width / 2, frame.origin.y + frame.size.height / 2);
}

static inline Vec2 alignedRight(Size body, Rect frame) {
    auto x = frame.origin.x + frame.size.width - body.width;
    auto y = frame.origin.y + frame.size.height - body.height;
    return Vec2(x, y);
}

static inline Vec2 rightOf(Size body, Rect frame) {
    auto x = frame.origin.x + frame.size.width + body.width;
    auto y = frame.origin.y + frame.size.height - body.height;
    return Vec2(x, y);
}

} }
