#pragma once

#include "cocos2d.h"
#include "flappy/utilities/Direction.hpp"

namespace flappy {
namespace geometry {

using cocos2d::Rect;
using cocos2d::Vec2;

static inline Vec2 topLeftOf(const Rect& frame) {
    return Vec2{frame.getMinX(), frame.getMaxY()};
}

static inline Vec2 topCenterOf(const Rect& frame) {
    return Vec2{frame.getMidX(), frame.getMaxY()};
}

static inline Vec2 topRightOf(const Rect& frame) {
    return Vec2{frame.getMaxX(), frame.getMaxY()};
}

static inline Vec2 centerLeftOf(const Rect& frame) {
    return Vec2{frame.getMinX(), frame.getMidY()};
}

static inline Vec2 centerOf(const Rect& frame) {
    return Vec2{frame.getMidX(), frame.getMidY()};
}

static inline Vec2 centerRightOf(const Rect& frame) {
    return Vec2{frame.getMaxX(), frame.getMidY()};
}

static inline Vec2 bottomLeftOf(const Rect& frame) {
    return Vec2{frame.getMinX(), frame.getMinY()};
}

static inline Vec2 bottomCenterOf(const Rect& frame) {
    return Vec2{frame.getMidX(), frame.getMinY()};
}

static inline Vec2 bottomRightOf(const Rect& frame) {
    return Vec2{frame.getMaxX(), frame.getMinY()};
}

static inline Vec2 bodyWidth(const Rect& frame) {
    return Vec2{frame.size.width / 2, 0};
}

static inline Vec2 bodyHeight(const Rect& frame) {
    return Vec2{0, frame.size.height / 2};
}

static inline Vec2 leftOf(const Rect& body, const Rect& frame) {
    return centerLeftOf(frame) - bodyWidth(body);
}

static inline Vec2 rightOf(const Rect& body, const Rect& frame) {
    return centerRightOf(frame) + bodyWidth(body);
}

static inline Vec2 above(const Rect& body, const Rect& frame) {
    return topCenterOf(frame) + bodyHeight(body);
}

static inline Vec2 below(const Rect& body, const Rect& frame) {
    return bottomCenterOf(frame) - bodyHeight(body);
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
