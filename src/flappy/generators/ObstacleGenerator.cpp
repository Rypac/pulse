#include "flappy/generators/ObstacleGenerator.hpp"
#include "flappy/utilities/Random.hpp"

using namespace flappy;

static float worldLength(cocos2d::Size world, Direction direction) {
    switch (direction) {
        case Direction::North:
        case Direction::South:
            return world.width;
        case Direction::East:
        case Direction::West:
            return world.height;
    }
}

Obstacle* ObstacleGenerator::generate() const {
    const auto direction = random::direction();
    const auto maxColumnLength = worldLength(world.size, direction) - Obstacle::gapLength;
    const auto bottomLength = random::between(0, maxColumnLength);
    const auto topLength = maxColumnLength - bottomLength;
    const auto obstacle = Obstacle::create(topLength, Obstacle::gapLength, bottomLength, direction);
    obstacle->positionInWorld(world);
    return obstacle;
}
