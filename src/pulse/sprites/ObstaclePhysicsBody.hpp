#pragma once

#include "cocos2d.h"

namespace pulse {

class Obstacle;

enum class ObstacleState {
    Incoming,
    Defeated
};

class ObstaclePhysicsBody : public cocos2d::PhysicsBody {
  public:
    ObstaclePhysicsBody(const Obstacle* obstacle);

    void defeat();

    ObstacleState currentState() const {
        return state_;
    }

  private:
    void setState(ObstacleState state);

    const Obstacle* const obstacle;
    ObstacleState state_;
};

}
