#include "pulse/sprites/ObstaclePhysicsBody.hpp"
#include "pulse/sprites/Obstacle.hpp"
#include "pulse/sprites/SpritePhysicsBody.hpp"

using namespace pulse;
using namespace cocos2d;

ObstaclePhysicsBody::ObstaclePhysicsBody(const Obstacle* obstacle): obstacle_{obstacle} {
    init();
    setState(State::Incoming);
}

void ObstaclePhysicsBody::defeat() {
    setState(State::Defeated);
}

void ObstaclePhysicsBody::setState(State newState) {
    auto applyPhysicsBody = [](Node* node, const std::function<PhysicsBody*(Size)>& bodyCreator) {
        node->setPhysicsBody(bodyCreator(node->getBoundingBox().size));
    };
    auto preventCollisions = [](Node* node) {
        physics_body::stopCollisions(node->getPhysicsBody());
    };
    switch (newState) {
        case State::Incoming:
            applyPhysicsBody(obstacle_->getTop(), physics_body::createObstacle);
            applyPhysicsBody(obstacle_->getBottom(), physics_body::createObstacle);
            applyPhysicsBody(obstacle_->getGap(), physics_body::createPath);
            break;
        case State::Defeated:
            preventCollisions(obstacle_->getTop());
            preventCollisions(obstacle_->getBottom());
            preventCollisions(obstacle_->getGap());
            break;
    }
    state_ = newState;
}
