#include "pulse/sprites/ObstacleIndicator.hpp"
#include "pulse/2d/Geometry.hpp"

using namespace cocos2d;
using namespace pulse;

ObstacleIndicator* ObstacleIndicator::create(Direction direction, const Rect& frame) {
    const auto column = new (std::nothrow) ObstacleIndicator{direction};
    if (column and column->initWithFrame(frame)) {
        column->autorelease();
        return column;
    }
    delete column;
    return nullptr;
}

bool ObstacleIndicator::initWithFrame(const Rect& frame) {
    if (not initWithFile(particleFilePath())) {
        return false;
    }

    setContentSize(indicatorSize(frame));
    setPosition(startingPosition(frame));

    return true;
}

std::string ObstacleIndicator::particleFilePath() const {
    switch (direction) {
        case Direction::North:
            return "particles/obstacle_indicator_bottom.plist";
        case Direction::South:
            return "particles/obstacle_indicator_top.plist";
        case Direction::East:
            return "particles/obstacle_indicator_left.plist";
        case Direction::West:
            return "particles/obstacle_indicator_right.plist";
    }
}

Size ObstacleIndicator::indicatorSize(const Rect& frame) const {
    switch (direction) {
        case Direction::North:
        case Direction::South:
            return Size{frame.size.width, thickness};
        case Direction::East:
        case Direction::West:
            return Size{thickness, frame.size.height};
    }
}

Vec2 ObstacleIndicator::startingPosition(const Rect& frame) const {
    switch (direction) {
        case Direction::North:
            return geometry::bottomCenterOf(frame);
        case Direction::South:
            return geometry::topCenterOf(frame);
        case Direction::East:
            return geometry::centerLeftOf(frame);
        case Direction::West:
            return geometry::centerRightOf(frame);
    }
}
