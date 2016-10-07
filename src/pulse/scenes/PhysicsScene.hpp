#pragma once

#include "pulse/scenes/BaseScene.hpp"

namespace pulse {

class PhysicsScene : public BaseScene {
  public:
    PhysicsScene();

    bool onScreenCollision(const cocos2d::PhysicsContact& contact) const;
};

}  // pulse
