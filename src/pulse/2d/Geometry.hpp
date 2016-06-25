#pragma once

#include "cocos2d.h"
#include "pulse/2d/Direction.hpp"

namespace pulse {
namespace geometry {

using cocos2d::Size;
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

static inline Vec2 origin(const Rect& body, const Rect& world, Direction direction) {
    switch (direction) {
        case Direction::North: return geometry::below(body, world);
        case Direction::South: return geometry::above(body, world);
        case Direction::East: return geometry::leftOf(body, world);
        case Direction::West: return geometry::rightOf(body, world);
    }
}

static inline Vec2 destination(const Rect& body, const Rect& world, Direction direction) {
    switch (direction) {
        case Direction::North: return geometry::above(body, world);
        case Direction::South: return geometry::below(body, world);
        case Direction::East: return geometry::rightOf(body, world);
        case Direction::West: return geometry::leftOf(body, world);
    }
}

static inline Size rotatedSize(const Size& size, float angle) {
    const auto width = size.width * std::cos(angle);
    const auto height = size.width * std::sin(angle);
    return Size{width, height};
}

static inline Vec2 rotatedOffset(const Size& size, float angle) {
    const auto x = size.height / 2.0f * std::sin(angle);
    const auto y = x * std::tan(angle);
    return Vec2{x, y};
}

static inline Vec2 leftEntryPosition(const Rect& frame, const Vec2& destination, const Size& size, float angle = 0.0f) {
    const auto radians = MATH_DEG_TO_RAD(angle);
    const auto sizeOffset = rotatedSize(size, radians);
    const auto offset = rotatedOffset(size, radians);
    const auto origin = Vec2{frame.getMinX() - offset.x, destination.y - (destination.x - frame.getMinX()) * std::tan(angle) - offset.y};

    return Vec2{origin.x - sizeOffset.width / 2.0f, origin.y - sizeOffset.height / 2.0f};
}

static inline Vec2 rightEntryPosition(const Rect& frame, const Vec2& destination, const Size& size, float angle = 0.0f) {
    const auto radians = MATH_DEG_TO_RAD(angle);
    const auto sizeOffset = rotatedSize(size, radians);
    const auto offset = rotatedOffset(size, radians);
    const auto origin = Vec2{frame.getMaxX() + offset.x, destination.y + (frame.getMaxX() - destination.x) * std::tan(angle) + offset.y};
    
    return Vec2{origin.x + sizeOffset.width / 2.0f, origin.y + sizeOffset.height / 2.0f};
}

static inline Vec2 entryPosition(Direction direction, const Rect& frame, const Vec2& destination, const Size& size, float angle = 0.0f) {
    switch (direction) {
        case Direction::East: return leftEntryPosition(frame, destination, size, angle);
        case Direction::West: return rightEntryPosition(frame, destination, size, angle);
        default: return Vec2::ZERO;
    }
}

} }
