#pragma once

#include "cocos2d.h"

namespace flappy { namespace rotation {

inline double distance(double x, double y) {
    return sqrt(x * x + y * y);
}

inline double roll(cocos2d::Vec3 point) {
    return atan(point.x / (distance(point.y, point.z) + MATH_FLOAT_SMALL));
}

inline double pitch(cocos2d::Vec3 point) {
    return atan(point.y / (distance(point.x, point.z) + MATH_FLOAT_SMALL));
}

inline double yaw(cocos2d::Vec3 point) {
    return atan(distance(point.x, point.y) / (point.z + MATH_FLOAT_SMALL));
}

inline double roll(cocos2d::Vec3 point, cocos2d::Vec3 reference) {
    return roll(point) - roll(reference);
}

inline double pitch(cocos2d::Vec3 point, cocos2d::Vec3 reference) {
    return pitch(point) - pitch(reference);
}

inline double yaw(cocos2d::Vec3 point, cocos2d::Vec3 reference) {
    return yaw(point) - yaw(reference);
}

inline cocos2d::Vec3 angle(cocos2d::Vec3 point) {
    return cocos2d::Vec3(roll(point), pitch(point), yaw(point));
}

} }
