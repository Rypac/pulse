#pragma once

#include "cocos2d.h"
#include "flappy/generators/Generator.hpp"
#include "flappy/sprites/Obstacle.hpp"

namespace flappy {

class ObstacleGenerator : public Generator<Obstacle*> {
  public:
    ObstacleGenerator() = delete;
    ObstacleGenerator(cocos2d::Rect world) : world(world) {};

    virtual Obstacle* generate() const;

  private:
    cocos2d::Rect world;
};

}
