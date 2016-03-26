#pragma once

#include "cocos2d.h"
#include "flappy/sprites/Column.hpp"
#include "flappy/utilities/Direction.hpp"

namespace flappy {

class Obstacle : public cocos2d::Sprite {
  public:
    CREATE_FUNC(Obstacle);
    static Obstacle* create(cocos2d::Size frame);
    static Obstacle* create(float topCollumnHeight, float gapHeight, float bottomCollumnHeight);

    virtual bool init() override;

    using ObstacleCallback = const std::function<void(Obstacle*)>;
    void runActions(cocos2d::Rect world, ObstacleCallback onCompletion = nullptr);
    void positionInWorld(cocos2d::Rect world);

    cocos2d::Vec2 origin(cocos2d::Rect world) const;
    cocos2d::Vec2 destination(cocos2d::Rect world) const;
    bool collidesWith(cocos2d::Rect frame) const;
    bool passedBy(cocos2d::Rect frame) const;

    static const int gapHeight = 100;

  private:
    Direction direction;
    Column* top;
    Column* bottom;
};

}
