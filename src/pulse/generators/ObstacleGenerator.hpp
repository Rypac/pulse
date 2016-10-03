#pragma once

#include "pulse/sprites/Obstacle.hpp"

namespace pulse {

class ObstacleGenerator {
  public:
    ObstacleGenerator() = delete;
    ObstacleGenerator(const cocos2d::Rect& world) : world(world) {};

    Obstacle* generate() const;

  private:
    cocos2d::Rect world;
};

}
