#pragma once

#include "cocos2d.h"

namespace pulse {
namespace rect {

using cocos2d::Rect;

inline cocos2d::Vec2 center(const Rect& rect) {
    return cocos2d::Vec2{rect.getMidX(), rect.getMidY()};
}

inline bool exceedsHorizontalBounds(const Rect& rect, const Rect& bounds) {
    return rect.getMaxX() > bounds.getMaxX() or rect.getMinX() < bounds.getMinX();
}

inline bool exceedsVerticalBounds(const Rect& rect, const Rect& bounds) {
    return rect.getMaxY() > bounds.getMaxY() or rect.getMinY() < bounds.getMinY();
}

inline bool exceedsBounds(const Rect& rect, const Rect& bounds) {
    return exceedsVerticalBounds(rect, bounds) or exceedsHorizontalBounds(rect, bounds);
}

inline bool withinHorizontalBounds(const Rect& rect, const Rect& bounds) {
    return rect.getMinX() > bounds.getMinX() and rect.getMaxX() < bounds.getMaxX();
}

inline bool withinVerticalBounds(const Rect& rect, const Rect& bounds) {
    return rect.getMinY() > bounds.getMinY() and rect.getMaxY() < bounds.getMaxY();
}

inline bool withinBounds(const Rect& rect, const Rect& bounds) {
    return withinHorizontalBounds(rect, bounds) and withinVerticalBounds(rect, bounds);
}

inline bool intersects(const Rect& rect1, const Rect& rect2) {
    return rect1.getMinX() < rect2.getMaxX() and rect1.getMaxX() > rect2.getMinX() and
        rect1.getMinY() < rect2.getMaxY() and rect1.getMaxY() > rect2.getMinY();
}

inline Rect intersection(const Rect& rect1, const Rect& rect2) {
    if (not intersects(rect1, rect2)) {
        return Rect::ZERO;
    }
    const auto x = std::max(rect1.getMinX(), rect2.getMinX());
    const auto y = std::max(rect1.getMinY(), rect2.getMinY());
    const auto width = std::min(rect1.getMaxX(), rect2.getMaxX()) - x;
    const auto height = std::min(rect1.getMaxY(), rect2.getMaxY()) - y;
    return Rect{x, y, width, height};
}

} }
