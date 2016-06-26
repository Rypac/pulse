#pragma once

#include "cocos2d.h"

namespace pulse {
namespace logger {

inline void log(const char* format, ...) {
    va_list args;
    va_start(args, format);
    cocos2d::log(format, args);
    va_end(args);
}

inline void log(float f) {
    cocos2d::log("%f", f);
}

inline void log(const cocos2d::Vec2& vec) {
    cocos2d::log("x = %f, y = %f", vec.x, vec.y);
}

inline void log(const cocos2d::Size& size) {
    cocos2d::log("width = %f, height = %f", size.width, size.height);
}

inline void log(const cocos2d::Rect& rect) {
    logger::log(rect.origin);
    logger::log(rect.size);
}

} }
