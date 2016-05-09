#include "pulse/sprites/ObstaclePhysicsBody.hpp"
#include "pulse/sprites/Obstacle.hpp"
#include "pulse/sprites/SpritePhysicsBody.hpp"

using namespace pulse;
using namespace cocos2d;

ObstaclePhysicsBody* ObstaclePhysicsBody::create(const Obstacle* obstacle) {
    const auto body = new (std::nothrow) ObstaclePhysicsBody(obstacle);
    if (body && body->init()) {
        body->autorelease();
        return body;
    }
    delete body;
    return nullptr;
}

bool ObstaclePhysicsBody::init() {
    if (!PhysicsBody::init()) {
        return false;
    }
    setState(currentState());
    return true;
}

void ObstaclePhysicsBody::defeat() {
    setState(ObstacleState::Defeated);
}

static void applyPhysicsBody(Node* node, std::function<PhysicsBody*(Size)> bodyCreator) {
    node->setPhysicsBody(bodyCreator(node->getBoundingBox().size));
}

static void preventCollisions(Node* node) {
    physics_body::stopCollisions(node->getPhysicsBody());
}

void ObstaclePhysicsBody::setState(ObstacleState newState) {
    switch (newState) {
        case ObstacleState::Incoming:
            applyPhysicsBody(obstacle->getTop(), physics_body::createObstacle);
            applyPhysicsBody(obstacle->getBottom(), physics_body::createObstacle);
            applyPhysicsBody(obstacle->getGap(), physics_body::createPath);
            break;
        case ObstacleState::Defeated:
            preventCollisions(obstacle->getTop());
            preventCollisions(obstacle->getBottom());
            preventCollisions(obstacle->getGap());
            break;
    }
    state = newState;
}
