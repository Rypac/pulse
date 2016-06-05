#pragma once

#include "pulse/scenes/GameScene.hpp"

namespace pulse {

class PhysicsScene : public GameScene {
  public:
    PhysicsScene();

    bool onScreenCollision(const cocos2d::PhysicsContact& contact) const;
};

}
