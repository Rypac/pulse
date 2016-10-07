#pragma once

#include "pulse/models/GameMode.hpp"
#include "pulse/models/TimeScale.hpp"

namespace pulse {

struct GameOptions {
    float obstacleFrequency{1};
    float obstacleFrequencyStep{0.1};
    float obstacleSpeed{3.5};
    float obstacleSpeedStep{0.2};
    int obstacleDefeatedThreshold{10};
    GameMode gameMode{GameMode::Classic};
    TimeScale slowMotionTimeScale{0.3, 0.3, 0.3};
    TimeScale reverseModeTimeScale{0.2, 0.05, 0.05};
};

}  // pulse
