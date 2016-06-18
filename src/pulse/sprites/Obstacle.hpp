#pragma once

#include "pulse/sprites/Column.hpp"
#include "pulse/utilities/Direction.hpp"

namespace pulse {

class ObstaclePhysicsBody;

class Obstacle : public cocos2d::Sprite {
  public:
    static Obstacle* create();
    static Obstacle* create(const cocos2d::Size& frame, Direction direction);
    static Obstacle* create(float topColumnLength, float gapLength, float bottomLength, Direction direction);

    virtual bool init() override;

    using ObstacleCallback = std::function<void(Obstacle*)>;
    ObstacleCallback onStarted;
    ObstacleCallback onCompletion;

    void runActions();
    void runDefeatedActions();
    void positionInWorld(const cocos2d::Rect& world);

    void setPhysicsBody(ObstaclePhysicsBody* body);
    ObstaclePhysicsBody* getPhysicsBody() const;

    static const int gapLength = 100;

    void setSpeed(float obstacleSpeed) {
        speed = obstacleSpeed;
    }

    float getSpeed() const {
        return speed;
    }

    Direction getDirection() const {
        return direction;
    }

    cocos2d::Vec2 getDestination() const {
        return destination;
    }

    Column* getTop() const {
        return top;
    }

    Column* getBottom() const {
        return bottom;
    }

    Node* getGap() const {
        return gap;
    }

  private:
    float speed;
    Direction direction;
    cocos2d::Vec2 destination;
    Column* top;
    Column* bottom;
    cocos2d::Node* gap;
};

}
