#pragma once

#include "cocos2d.h"

namespace pulse {
namespace position {

using cocos2d::Rect;
using cocos2d::Vec2;

static inline float normalisedHoriztonal(const Rect& rect, const Rect& bounds) {
    if (rect.getMinX() > bounds.getMaxX()) {
        return rect.getMinX() - bounds.size.width;
    } else if (rect.getMaxX() < bounds.getMinX()) {
        return rect.getMinX() + bounds.size.width;
    }
    return rect.getMinX();
}

static inline float normalisedVertical(const Rect& rect, const Rect& bounds) {
    if (rect.getMinY() > bounds.getMaxY()) {
        return rect.getMinY() - bounds.size.height;
    } else if (rect.getMaxY() < bounds.getMinY()) {
        return rect.getMinY() + bounds.size.height;
    }
    return rect.getMinY();
}

static inline Vec2 normalised(const Rect& body, const Rect& bounds) {
    return Vec2{normalisedHoriztonal(body, bounds), normalisedVertical(body, bounds)};
}

static inline float mirroredHoriztonal(const Rect& rect, const Rect& bounds) {
    if (rect.getMaxX() > bounds.getMaxX()) {
        return rect.getMinX() - bounds.size.width;
    } else if (rect.getMinX() < bounds.getMinX()) {
        return rect.getMinX() + bounds.size.width;
    }
    return rect.getMinX();
}

static inline float mirroredVertical(const Rect& rect, const Rect& bounds) {
    if (rect.getMaxY() > bounds.getMaxY()) {
        return rect.getMinY() - bounds.size.height;
    } else if (rect.getMinY() < bounds.getMinY()) {
        return rect.getMinY() + bounds.size.height;
    }
    return rect.getMinY();
}

static inline Vec2 mirrored(const Rect& body, const Rect& bounds) {
    return Vec2{mirroredHoriztonal(body, bounds), mirroredVertical(body, bounds)};
}

} }
