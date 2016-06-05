#include "pulse/scenes/PhysicsScene.hpp"

#include "range/v3/algorithm/any_of.hpp"

using namespace cocos2d;
using namespace pulse;

bool PhysicsScene::init() {
    return initWithPhysics();
}

bool PhysicsScene::onScreenCollision(const PhysicsContact& contact) const {
    return ranges::any_of(contact.getContactData()->points, [this](auto point) {
        return point != Vec2::ZERO && this->sceneFrame().containsPoint(point);
    });
}
