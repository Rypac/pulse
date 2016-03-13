#pragma once

#include "cocos2d.h"
#include "flappy/sprites/Column.hpp"

namespace flappy {

class Obstacle : public cocos2d::Sprite {
  public:
    CREATE_FUNC(Obstacle);
    static Obstacle* create(cocos2d::Size frame);
    static Obstacle* create(float topCollumnHeight, float gapHeight, float bottomCollumnHeight);

    virtual bool init() override;

    using ObstacleCallback = const std::function<void(Obstacle*)>;
    void runActions(ObstacleCallback onCompletion = nullptr);

    bool collidesWith(cocos2d::Rect frame) const;
    bool passedBy(cocos2d::Rect frame) const;

    static const int gapHeight = 100;

  private:
    Column* top;
    Column* bottom;
};

}
