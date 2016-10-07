#pragma once

#include "cocos2d.h"
#include "pulse/2d/Direction.hpp"

namespace pulse {

class ObstacleIndicator : public cocos2d::ParticleSystemQuad {
  public:
    static ObstacleIndicator* create(Direction direction, const cocos2d::Rect& frame);

    cocos2d::Vec2 startingPosition(const cocos2d::Rect& frame) const;
    cocos2d::Size indicatorSize(const cocos2d::Rect& frame) const;
    std::string particleFilePath() const;

  protected:
    ObstacleIndicator(Direction direction)
    : direction{direction} {}

    virtual bool initWithFrame(const cocos2d::Rect& frame);

  private:
    const Direction direction;
    const float thickness{80};
};

}  // pulse
