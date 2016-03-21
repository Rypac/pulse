#pragma once

#include "cocos2d.h"

namespace flappy { namespace rotation {

inline double angle(cocos2d::Vec2 p2, cocos2d::Vec2 p1 = cocos2d::Vec2{1, 0}) {
    return asin((p1.x * p2.y - p2.x * p1.y) / (p1.x * p1.x + p1.y * p1.y + MATH_EPSILON));
}

inline double pitch(cocos2d::Vec3 p2, cocos2d::Vec3 p1) {
    return angle(cocos2d::Vec2{p2.y, p2.z}, cocos2d::Vec2{p1.y, p1.z});
}

inline double yaw(cocos2d::Vec3 p2, cocos2d::Vec3 p1) {
    return angle(cocos2d::Vec2{p2.x, p2.z}, cocos2d::Vec2{p1.x, p1.z});
}

inline double roll(cocos2d::Vec3 p2, cocos2d::Vec3 p1) {
    return angle(cocos2d::Vec2{p2.x, p2.y}, cocos2d::Vec2{p1.x, p1.y});
}

} }
