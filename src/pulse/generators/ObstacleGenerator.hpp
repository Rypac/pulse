#pragma once

#include "cocos2d.h"
#include "pulse/generators/Generator.hpp"
#include "pulse/sprites/Obstacle.hpp"

namespace pulse {

class ObstacleGenerator : public Generator<Obstacle*> {
  public:
    ObstacleGenerator() = delete;
    ObstacleGenerator(cocos2d::Rect world) : world(world) {};

    virtual Obstacle* generate() const override;

  private:
    cocos2d::Rect world;
};

}
