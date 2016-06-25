#include "pulse/scenes/PhysicsScene.hpp"

#include <range/v3/algorithm/any_of.hpp>

using namespace cocos2d;
using namespace pulse;

PhysicsScene::PhysicsScene() {
    initWithPhysics();
}

bool PhysicsScene::onScreenCollision(const PhysicsContact& contact) const {
    return ranges::any_of(contact.getContactData()->points, [&frame = sceneFrame()](auto point) {
        return point != Vec2::ZERO and frame.containsPoint(point);
    });
}
