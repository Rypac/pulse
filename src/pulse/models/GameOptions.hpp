#pragma once

#include "cocos2d.h"
#include "pulse/models/GameMode.hpp"
#include "pulse/models/TimeScale.hpp"

namespace pulse {

struct GameOptions {
    float obstacleFrequency{1};
    float obstacleSpeed{3.5};
    GameMode gameMode{GameMode::Classic};
    TimeScale slowMotionTimeScale{1.0, 0.3};
};

}
