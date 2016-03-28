#pragma once

#include "cocos2d.h"
#include "flappy/sprites/Column.hpp"
#include "flappy/utilities/Direction.hpp"

namespace flappy {

class Obstacle : public cocos2d::Sprite {
  public:
    CREATE_FUNC(Obstacle);
    static Obstacle* create(cocos2d::Size frame, Direction direction);
    static Obstacle* create(float topColumnLength, float gapLength, float bottomLength, Direction direction);

    virtual bool init() override;

    Direction getDirection() const { return direction; };
    cocos2d::Vec2 getDestination() const { return destination; };

    using ObstacleCallback = const std::function<void(Obstacle*)>;
    void runActions(ObstacleCallback onCompletion = nullptr);
    void runDefeatedActions(ObstacleCallback onCompletion = nullptr);
    void positionInWorld(cocos2d::Rect world);

    static const int gapLength = 100;

    Column* getTop() const { return top; };
    Column* getBottom() const { return bottom; };
    Node* getGap() const { return gap; };

  private:
    Direction direction;
    cocos2d::Vec2 destination;
    Column* top;
    Column* bottom;
    cocos2d::Node* gap;
};

}
