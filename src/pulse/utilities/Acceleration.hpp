#pragma once

#include "cocos2d.h"

namespace pulse {
namespace accelerometer {

using cocos2d::Acceleration;

inline Acceleration filter(const Acceleration& acceleration, const Acceleration& previous = Acceleration()) {
    const auto filteringFactor = 0.75;
    auto filtered = Acceleration();
    filtered.x = (filteringFactor * acceleration.x) + (1.0 - filteringFactor) * previous.x;
    filtered.y = (filteringFactor * acceleration.y) + (1.0 - filteringFactor) * previous.y;
    filtered.z = (filteringFactor * acceleration.z) + (1.0 - filteringFactor) * previous.z;
    return filtered;
}

} }
