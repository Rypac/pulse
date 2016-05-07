#include "pulse/sprites/ObstacleIndicator.hpp"

using namespace cocos2d;
using namespace pulse;

ObstacleIndicator* ObstacleIndicator::create() {
    ObstacleIndicator *column = new (std::nothrow) ObstacleIndicator();
    if (column && column->init()) {
        column->autorelease();
        return column;
    }
    delete column;
    return nullptr;
}
