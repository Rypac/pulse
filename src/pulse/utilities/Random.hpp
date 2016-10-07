#pragma once

#include "pulse/2d/Direction.hpp"

namespace pulse {
namespace random {

inline int between(int min, int max) {
    std::random_device seed;
    std::default_random_engine randomEngine(seed());
    std::uniform_int_distribution<int> uniformDistribution(min, max);
    return uniformDistribution(randomEngine);
}

inline Direction direction() {
    switch (random::between(0, 3)) {
        case 0:
            return Direction::North;
        case 1:
            return Direction::South;
        case 2:
            return Direction::East;
        case 3:
            return Direction::West;
        default:
            return Direction::North;
    }
}

}  // random
}  // pulse
