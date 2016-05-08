#pragma once

#include "cocos2d.h"
#include "pulse/sprites/Obstacle.hpp"
#include "pulse/sprites/ObstacleIndicator.hpp"

namespace pulse {

class ObstacleSequence : public cocos2d::Sequence {
  public:
    static ObstacleSequence* create(Obstacle* obstacle, float indicatorDuration = 2);

  protected:
    ObstacleSequence(Obstacle* obstacle, float indicatorDuration):
        obstacle{obstacle},
        indicatorDuration{indicatorDuration} {}
    ~ObstacleSequence();

    virtual bool init();

  private:
    Obstacle* obstacle;
    ObstacleIndicator* indicator;
    const float indicatorDuration;
};

}
