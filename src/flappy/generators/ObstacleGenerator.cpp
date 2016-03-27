#include "flappy/generators/ObstacleGenerator.hpp"
#include "flappy/utilities/Random.hpp"

using namespace flappy;

Obstacle* ObstacleGenerator::generate() const {
    const auto direction = Direction::West;
    const auto maxColumnLength = world.size.height - Obstacle::gapLength;
    const auto bottomLength = random::between(0, maxColumnLength);
    const auto topLength = maxColumnLength - bottomLength;
    const auto obstacle = Obstacle::create(topLength, Obstacle::gapLength, bottomLength, direction);
    obstacle->positionInWorld(world);
    return obstacle;
}
