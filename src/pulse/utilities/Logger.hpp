#pragma once

#include "cocos2d.h"

namespace pulse {
namespace logger {

static inline void log(const char* format, ...) {
    va_list args;
    va_start(args, format);
    cocos2d::log(format, args);
    va_end(args);
}

static inline void log(const cocos2d::Vec2& vec) {
    cocos2d::log("x = %f, y = %f", vec.x, vec.y);
}

static inline void log(const cocos2d::Size& size) {
    cocos2d::log("width = %f, height = %f", size.width, size.height);
}

static inline void log(const cocos2d::Rect& rect) {
    logger::log(rect.origin);
    logger::log(rect.size);
}

} }
