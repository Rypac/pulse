#pragma once

#include "cocos2d.h"

namespace pulse {
namespace rotation {

inline float distance(float x, float y) {
    return sqrt(x * x + y * y);
}

inline float roll(const cocos2d::Vec3& point) {
    return atan(point.x / (distance(point.y, point.z) + MATH_FLOAT_SMALL));
}

inline float pitch(const cocos2d::Vec3& point) {
    return atan(point.y / (distance(point.x, point.z) + MATH_FLOAT_SMALL));
}

inline float yaw(const cocos2d::Vec3& point) {
    return atan(distance(point.x, point.y) / (point.z + MATH_FLOAT_SMALL));
}

inline float roll(const cocos2d::Vec3& point, const cocos2d::Vec3& reference) {
    return roll(point) - roll(reference);
}

inline float pitch(const cocos2d::Vec3& point, const cocos2d::Vec3& reference) {
    return pitch(point) - pitch(reference);
}

inline float yaw(const cocos2d::Vec3& point, const cocos2d::Vec3& reference) {
    return yaw(point) - yaw(reference);
}

inline cocos2d::Vec3 angle(const cocos2d::Vec3& point) {
    return cocos2d::Vec3{roll(point), pitch(point), yaw(point)};
}

}  // rotation
}  // pulse
