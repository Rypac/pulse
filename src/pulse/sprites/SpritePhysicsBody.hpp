#pragma once

#include "cocos2d.h"
#include "pulse/extensions/Optional.hpp"

namespace pulse {
namespace physics_body {

cocos2d::PhysicsBody* createHero(const cocos2d::Size& size);
cocos2d::PhysicsBody* createPath(const cocos2d::Size& size);
cocos2d::PhysicsBody* createObstacle(const cocos2d::Size& size);
cocos2d::PhysicsBody* clone(cocos2d::PhysicsBody* body, const cocos2d::Size& size);

bool isHero(const cocos2d::PhysicsBody& body);
bool isPath(const cocos2d::PhysicsBody& body);
bool isObstacle(const cocos2d::PhysicsBody& body);
void stopCollisions(cocos2d::PhysicsBody* body);

using NodePredicate = std::function<bool(const cocos2d::PhysicsBody&)>;
std::optional<cocos2d::Node*> nodeInContact(const cocos2d::PhysicsBody& body1, const cocos2d::PhysicsBody& body2, const NodePredicate isNode);
std::optional<cocos2d::Node*> nodeInContact(const cocos2d::PhysicsContact& contact, const NodePredicate isNode);

namespace collision {

    bool heroAndObstacle(const cocos2d::PhysicsContact& contact);
    bool heroAndObstacle(const cocos2d::PhysicsBody& body1, const cocos2d::PhysicsBody& body2);
    bool heroAndPath(const cocos2d::PhysicsContact& contact);
    bool heroAndPath(const cocos2d::PhysicsBody& body1, const cocos2d::PhysicsBody& body2);

}

} }
