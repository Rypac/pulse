#pragma once

#include "pulse/sprites/Obstacle.hpp"
#include "pulse/sprites/ObstacleIndicator.hpp"

namespace pulse {

class ObstacleSequence : public cocos2d::Sequence {
  public:
    ObstacleSequence(Obstacle* obstacle, float indicatorDuration);
    virtual ~ObstacleSequence();

  private:
    Obstacle* obstacle_;
    ObstacleIndicator* indicator_;
};

}  // pulse
