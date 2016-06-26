#pragma once

#include "cocos2d.h"

namespace pulse {
namespace vec {

using cocos2d::Vec3;

inline Vec3 scaleBy(const Vec3& vec, const Vec3& scale) {
    return Vec3{vec.x * scale.x, vec.y * scale.y, vec.z * scale.z};
}

} }
