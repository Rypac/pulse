#pragma once

#include "cocos2d.h"

namespace pulse {
namespace vec {

inline cocos2d::Vec3 scaleBy(const cocos2d::Vec3& vec, const cocos2d::Vec3& scale) {
    return cocos2d::Vec3{vec.x * scale.x, vec.y * scale.y, vec.z * scale.z};
}

}  // vec
}  // pulse
