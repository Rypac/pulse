#pragma once

#include "cocos2d.h"
#include "pulse/models/TimeScale.hpp"

namespace pulse {

struct GameOptions {
    cocos2d::Vec2 accelerometerSensitivity{2200, 1500};
    float obstacleFrequency{1};
    float obstacleSpeed{3.5};
    TimeScale slowMotionTimeScale{0.4, 0.3};
};

}
