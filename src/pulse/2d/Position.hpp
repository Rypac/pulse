#pragma once

#include "cocos2d.h"

namespace pulse {
namespace position {

using cocos2d::Rect;
using cocos2d::Size;
using cocos2d::Vec2;

inline float normalisedHorizontal(const Rect& rect, const Rect& bounds) {
    const auto x = fmod(rect.getMinX(), bounds.getMaxX());
    return rect.getMaxX() < bounds.getMinX() ? x + bounds.getMaxX() : x;
}

inline float normalisedVertical(const Rect& rect, const Rect& bounds) {
    const auto y = fmod(rect.getMinY(), bounds.getMaxY());
    return rect.getMaxY() < bounds.getMinY() ? y + bounds.getMaxY() : y;
}

inline Vec2 normalised(const Rect& body, const Rect& bounds) {
    return Vec2{normalisedHorizontal(body, bounds), normalisedVertical(body, bounds)};
}

inline float mirroredHorizontal(const Rect& rect, const Rect& bounds) {
    const auto x = fmod(rect.getMaxX(), bounds.getMaxX()) - rect.size.width;
    return rect.getMinX() < bounds.getMinX() ? x + bounds.getMaxX() : x;
}

inline float mirroredVertical(const Rect& rect, const Rect& bounds) {
    const auto y = fmod(rect.getMaxY(), bounds.getMaxY()) - rect.size.height;
    return rect.getMinY() < bounds.getMinY() ? y + bounds.getMaxY() : y;
}

inline Vec2 mirrored(const Rect& body, const Rect& bounds) {
    return Vec2{mirroredHorizontal(body, bounds), mirroredVertical(body, bounds)};
}

inline Vec2 relativeToAnchorPoint(const Vec2& position, const Size& size, const Vec2& anchorPoint) {
    auto relativePosition = position;
    relativePosition.x += (size.width * anchorPoint.x);
    relativePosition.y += (size.height * anchorPoint.y);
    return relativePosition;
}

} }
