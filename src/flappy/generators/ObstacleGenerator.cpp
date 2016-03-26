#include "flappy/generators/ObstacleGenerator.hpp"
#include "flappy/utilities/Random.hpp"

using namespace flappy;

Obstacle* ObstacleGenerator::generate() const {
    const auto maxColumnHeight = world.size.height - Obstacle::gapHeight;
    const auto bottomHeight = random::between(0, maxColumnHeight);
    const auto topHeight = maxColumnHeight - bottomHeight;
    const auto obstacle = Obstacle::create(topHeight, Obstacle::gapHeight, bottomHeight);
    obstacle->positionInWorld(world);
    return obstacle;
}
