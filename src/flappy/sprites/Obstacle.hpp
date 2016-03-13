#pragma once

#include <gsl/gsl.h>

#include "cocos2d.h"
#include "flappy/sprites/Column.hpp"

namespace flappy {

struct Obstacle {

  public:
    Obstacle() = delete;
    Obstacle(gsl::not_null<Column*> top, gsl::not_null<Column*> bottom) : top(top), bottom(bottom) {}

    using ObstacleCallback = std::function<void(Obstacle)>;
    void runActions(ObstacleCallback onCompletion) const;

    bool collidesWith(cocos2d::Rect frame);
    bool passedBy(cocos2d::Rect frame);

    const gsl::not_null<Column*> top;
    const gsl::not_null<Column*> bottom;
};

}
