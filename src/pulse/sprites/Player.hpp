#pragma once

#include "pulse/sprites/WrappedSprite.hpp"

namespace pulse {

class Player : public WrappedSprite {
  public:
    static Player* create();

    virtual void update(float dt) override;

    const cocos2d::Vec2& velocity() const;
    void setVelocity(const cocos2d::Vec2& velocity);

  private:
    Player()
    : velocity_{cocos2d::Vec2::ZERO} {}

    cocos2d::Vec2 velocity_;
};

}  // pulse
