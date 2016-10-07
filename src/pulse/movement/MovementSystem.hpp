#pragma once

#include "cocos2d.h"

namespace pulse {

struct MovementSystem {
    std::function<void(const cocos2d::Vec3& movementVector)> onMovement;
};

}  // pulse
