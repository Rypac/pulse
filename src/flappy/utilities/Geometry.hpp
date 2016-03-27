#pragma once

#include "cocos2d.h"
#include "flappy/utilities/Direction.hpp"

namespace flappy { namespace geometry {

using cocos2d::Rect;
using cocos2d::Vec2;

static inline Vec2 centerOf(Rect frame) {
    return Vec2{frame.origin.x + frame.size.width / 2, frame.origin.y + frame.size.height / 2};
}

static inline Vec2 alignedLeft(Rect body, Rect frame) {
    return Vec2{frame.origin.x, frame.origin.y + frame.size.height - body.size.height};
}

static inline Vec2 alignedRight(Rect body, Rect frame) {
    return Vec2{frame.origin.x + frame.size.width - body.size.width, frame.origin.y + frame.size.height - body.size.height};
}

static inline Vec2 alignedTop(Rect body, Rect frame) {
    return Vec2{body.origin.x, frame.origin.y + frame.size.height - body.size.height};
}

static inline Vec2 alignedBottom(Rect body, Rect frame) {
    return Vec2{body.origin.x, frame.origin.y};
}

static inline Vec2 leftOf(Rect body, Rect frame) {
    return Vec2{frame.origin.x - body.size.width, frame.origin.y + frame.size.height - body.size.height};
}

static inline Vec2 rightOf(Rect body, Rect frame) {
    return Vec2{frame.origin.x + frame.size.width + body.size.width, frame.origin.y + frame.size.height - body.size.height};
}

static inline Vec2 above(Rect body, Rect frame) {
    return Vec2{body.origin.x, frame.origin.y + frame.size.height + body.size.height};
}

static inline Vec2 below(Rect body, Rect frame) {
    return Vec2{body.origin.x, frame.origin.y - body.size.height};
}

static inline Vec2 origin(Rect body, Rect world, Direction direction) {
    switch (direction) {
        case Direction::North: return geometry::below(body, world);
        case Direction::South: return geometry::above(body, world);
        case Direction::East: return geometry::leftOf(body, world);
        case Direction::West: return geometry::rightOf(body, world);
    }
}

static inline Vec2 destination(Rect body, Rect world, Direction direction) {
    switch (direction) {
        case Direction::North: return geometry::above(body, world);
        case Direction::South: return geometry::below(body, world);
        case Direction::East: return geometry::rightOf(body, world);
        case Direction::West: return geometry::leftOf(body, world);
    }
}

} }
