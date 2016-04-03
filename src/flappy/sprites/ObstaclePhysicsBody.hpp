#pragma once

#include "cocos2d.h"

namespace flappy {

class Obstacle;

enum class ObstacleState {
    Incoming,
    Passed,
    Defeated
};

class ObstaclePhysicsBody : public cocos2d::PhysicsBody {
  public:
    ObstaclePhysicsBody(): state{ObstacleState::Incoming} {}

    static ObstaclePhysicsBody* create(Obstacle* obstacle);

    void pass();
    void defeat();

    ObstacleState currentState() const {
        return state;
    }

  private:
    void setState(ObstacleState state);

    Obstacle* obstacle;
    ObstacleState state;
};

}
