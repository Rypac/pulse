#pragma once

namespace flappy {

enum class SpriteTag : int {
    None = 0,
    Hero = (1 << 0),
    Obstacle = (1 << 1),
    Path = (2 << 1)
};

}
