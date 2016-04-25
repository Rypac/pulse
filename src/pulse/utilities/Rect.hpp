#pragma once

#include "cocos2d.h"

namespace pulse {
namespace rect {

using cocos2d::Rect;

static inline bool exceedsHorizontalBounds(const Rect& rect, const Rect& bounds) {
    return rect.getMaxX() > bounds.getMaxX() || rect.getMinX() < bounds.getMinX();
}

static inline bool exceedsVerticalBounds(const Rect& rect, const Rect& bounds) {
    return rect.getMaxY() > bounds.getMaxY() || rect.getMinY() < bounds.getMinY();
}

static inline bool exceedsBounds(const Rect& rect, const Rect& bounds) {
    return exceedsVerticalBounds(rect, bounds) || exceedsHorizontalBounds(rect, bounds);
}

static inline bool withinHorizontalBounds(const Rect& rect, const Rect& bounds) {
    return rect.getMinX() > bounds.getMinX() && rect.getMaxX() < bounds.getMaxX();
}

static inline bool withinVerticalBounds(const Rect& rect, const Rect& bounds) {
    return rect.getMinY() > bounds.getMinY() && rect.getMaxY() < bounds.getMaxY();
}

static inline bool withinBounds(const Rect& rect, const Rect& bounds) {
    return withinHorizontalBounds(rect, bounds) && withinVerticalBounds(rect, bounds);
}

} }
