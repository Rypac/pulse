#pragma once

#include "pulse/scenes/GameScene.hpp"

namespace pulse {

class PhysicsScene : public GameScene {
  public:
    virtual bool init() override;

    bool onScreenCollision(const cocos2d::PhysicsContact& contact) const;
};

}
