#pragma once

#include "cocos2d.h"

namespace flappy {
namespace physics_body {

cocos2d::PhysicsBody* createHero(cocos2d::Size size);
cocos2d::PhysicsBody* createPath(cocos2d::Size size);
cocos2d::PhysicsBody* createObstacle(cocos2d::Size size);

bool isHero(const cocos2d::PhysicsBody& body);
bool isPath(const cocos2d::PhysicsBody& body);
bool isObstacle(const cocos2d::PhysicsBody& body);

namespace collision {

    bool heroAndObstacle(const cocos2d::PhysicsContact& contact);
    bool heroAndObstacle(const cocos2d::PhysicsBody& body1, const cocos2d::PhysicsBody& body2);
    bool heroAndPath(const cocos2d::PhysicsContact& contact);
    bool heroAndPath(const cocos2d::PhysicsBody& body1, const cocos2d::PhysicsBody& body2);

} } }
