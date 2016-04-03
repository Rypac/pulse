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
    ObstaclePhysicsBody(const Obstacle* obstacle): state{ObstacleState::Incoming}, obstacle{obstacle} {}

    static ObstaclePhysicsBody* create(const Obstacle* obstacle);

    bool init() override;
    void pass();
    void defeat();

    ObstacleState currentState() const {
        return state;
    }

  private:
    void setState(ObstacleState state);

    const Obstacle* const obstacle;
    ObstacleState state;
};

}
