#pragma once

#include "cocos2d.h"

namespace pulse {

class Obstacle;

class ObstaclePhysicsBody : public cocos2d::PhysicsBody {
  public:
    ObstaclePhysicsBody(const Obstacle* obstacle);

    void defeat();

  private:
    enum class State {
        Incoming,
        Defeated
    };

    void setState(State state);

    const Obstacle* const obstacle_;
    State state_;
};

}  // pulse
