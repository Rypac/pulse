#pragma once

#include "pulse/scenes/GameScene.hpp"

namespace pulse {

class PhysicsScene : public GameScene {
  public:
    static cocos2d::Scene* createScene(GameScene* scene, cocos2d::Vec2 gravity = cocos2d::Vec2::ZERO, bool autoStep = false);

    bool onScreenCollision(const cocos2d::PhysicsContact& contact) const;
};

}
