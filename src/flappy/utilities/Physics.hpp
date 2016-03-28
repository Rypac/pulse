#pragma once

#include "cocos2d.h"
#include "flappy/sprites/SpriteTag.hpp"

namespace flappy { namespace physics {

static const auto Gravity = cocos2d::Vec2{0, -400};
static const auto AccelerometerScale = 1200;

namespace detail {
    static inline bool isCategory(const cocos2d::PhysicsBody& body, SpriteTag tag) {
        return body.getCategoryBitmask() == static_cast<int>(tag);
    }
}

static inline bool isHero(const cocos2d::PhysicsBody& body) {
    return detail::isCategory(body, SpriteTag::Hero);
}

static inline bool isObstacle(const cocos2d::PhysicsBody& body) {
    return detail::isCategory(body, SpriteTag::Obstacle);
}

static inline bool isPath(const cocos2d::PhysicsBody& body) {
    return detail::isCategory(body, SpriteTag::Path);
}

static bool isHeroAndObstacleCollision(const cocos2d::PhysicsBody& body1, const cocos2d::PhysicsBody& body2) {
    return isHero(body1) || isHero(body2) ? isObstacle(body1) || isObstacle(body2) : false;
}

static bool isHeroAndPathCollision(const cocos2d::PhysicsBody& body1, const cocos2d::PhysicsBody& body2) {
    return isHero(body1) || isHero(body2) ? isPath(body1) || isPath(body2) : false;
}

} }
