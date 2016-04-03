#pragma once

#include "cocos2d.h"
#include "flappy/sprites/Column.hpp"
#include "flappy/utilities/Direction.hpp"

namespace flappy {

class ObstaclePhysicsBody;

class Obstacle : public cocos2d::Sprite {
  public:
    CREATE_FUNC(Obstacle);
    static Obstacle* create(cocos2d::Size frame, Direction direction);
    static Obstacle* create(float topColumnLength, float gapLength, float bottomLength, Direction direction);

    virtual bool init() override;

    using ObstacleCallback = std::function<void(Obstacle*)>;
    ObstacleCallback onCompletion;

    void runActions(float duration);
    void runDefeatedActions();
    void positionInWorld(cocos2d::Rect world);

    void setPhysicsBody(ObstaclePhysicsBody* body);
    ObstaclePhysicsBody* getPhysicsBody() const;

    static const int gapLength = 100;

    Direction getDirection() const {
        return direction;
    };

    cocos2d::Vec2 getDestination() const {
        return destination;
    };

    Column* getTop() const {
        return top;
    };

    Column* getBottom() const {
        return bottom;
    };

    Node* getGap() const {
        return gap;
    };

  private:
    Direction direction;
    cocos2d::Vec2 destination;
    Column* top;
    Column* bottom;
    cocos2d::Node* gap;
};

}
