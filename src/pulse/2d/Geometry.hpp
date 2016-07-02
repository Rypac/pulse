#pragma once

#include "cocos2d.h"
#include "pulse/2d/Direction.hpp"

namespace pulse {
namespace geometry {

using cocos2d::Size;
using cocos2d::Rect;
using cocos2d::Vec2;

inline Vec2 topLeftOf(const Rect& frame) {
    return Vec2{frame.getMinX(), frame.getMaxY()};
}

inline Vec2 topCenterOf(const Rect& frame) {
    return Vec2{frame.getMidX(), frame.getMaxY()};
}

inline Vec2 topRightOf(const Rect& frame) {
    return Vec2{frame.getMaxX(), frame.getMaxY()};
}

inline Vec2 centerLeftOf(const Rect& frame) {
    return Vec2{frame.getMinX(), frame.getMidY()};
}

inline Vec2 centerOf(const Rect& frame) {
    return Vec2{frame.getMidX(), frame.getMidY()};
}

inline Vec2 centerRightOf(const Rect& frame) {
    return Vec2{frame.getMaxX(), frame.getMidY()};
}

inline Vec2 bottomLeftOf(const Rect& frame) {
    return Vec2{frame.getMinX(), frame.getMinY()};
}

inline Vec2 bottomCenterOf(const Rect& frame) {
    return Vec2{frame.getMidX(), frame.getMinY()};
}

inline Vec2 bottomRightOf(const Rect& frame) {
    return Vec2{frame.getMaxX(), frame.getMinY()};
}

inline Vec2 bodyWidth(const Rect& frame) {
    return Vec2{frame.size.width / 2, 0};
}

inline Vec2 bodyHeight(const Rect& frame) {
    return Vec2{0, frame.size.height / 2};
}

inline Vec2 leftOf(const Rect& body, const Rect& frame) {
    return centerLeftOf(frame) - bodyWidth(body);
}

inline Vec2 rightOf(const Rect& body, const Rect& frame) {
    return centerRightOf(frame) + bodyWidth(body);
}

inline Vec2 above(const Rect& body, const Rect& frame) {
    return topCenterOf(frame) + bodyHeight(body);
}

inline Vec2 below(const Rect& body, const Rect& frame) {
    return bottomCenterOf(frame) - bodyHeight(body);
}

inline Vec2 origin(const Rect& body, const Rect& world, Direction direction) {
    switch (direction) {
        case Direction::North: return geometry::below(body, world);
        case Direction::South: return geometry::above(body, world);
        case Direction::East: return geometry::leftOf(body, world);
        case Direction::West: return geometry::rightOf(body, world);
    }
}

inline Vec2 destination(const Rect& body, const Rect& world, Direction direction) {
    switch (direction) {
        case Direction::North: return geometry::above(body, world);
        case Direction::South: return geometry::below(body, world);
        case Direction::East: return geometry::rightOf(body, world);
        case Direction::West: return geometry::leftOf(body, world);
    }
}

inline Size rotatedSize(const Size& size, float angle) {
    const auto width = size.width * std::cos(angle);
    const auto height = size.width * std::sin(angle);
    return Size{width, height};
}

inline Vec2 rotatedOffset(const Size& size, float angle) {
    const auto x = size.height / 2.0f * std::sin(angle);
    const auto y = x * std::tan(angle);
    return Vec2{x, y};
}

inline Vec2 entryPosition(Direction direction, const Rect& frame, const Vec2& destination, const Size& size, float angle = 0.0f) {
    const auto radians = MATH_DEG_TO_RAD(angle);
    const auto sizeOffset = rotatedSize(size, radians);
    const auto offset = rotatedOffset(size, radians);

    switch (direction) {
        case Direction::West: {
            const auto origin = Vec2{frame.getMinX() - offset.x, destination.y - (destination.x - frame.getMinX()) * std::tan(radians) - offset.y};
            return Vec2{origin.x - sizeOffset.width / 2.0f, origin.y - sizeOffset.height / 2.0f};
        }
        case Direction::East: {
            const auto origin = Vec2{frame.getMaxX() + offset.x, destination.y + (frame.getMaxX() - destination.x) * std::tan(radians) + offset.y};
            return Vec2{origin.x + sizeOffset.width / 2.0f, origin.y + sizeOffset.height / 2.0f};
        }
        default:
            return Vec2::ZERO;
    }
}

inline Vec2 entryPosition(Direction direction, const Rect& frame, const Vec2& destination, const cocos2d::Node* node) {
    return entryPosition(direction, frame, destination, node->getContentSize(), -node->getRotation());
}

} }
