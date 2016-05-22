#include "pulse/scenes/PhysicsScene.hpp"

#include "range/v3/algorithm/any_of.hpp"

using namespace cocos2d;
using namespace pulse;

Scene* PhysicsScene::createScene(GameScene* gameScene, Vec2 gravity, bool autoStep) {
    const auto scene = Scene::createWithPhysics();
    scene->getPhysicsWorld()->setGravity(gravity);
    scene->getPhysicsWorld()->setAutoStep(autoStep);
    scene->addChild(gameScene);
    return scene;
}

bool PhysicsScene::onScreenCollision(const PhysicsContact& contact) const {
    return ranges::any_of(contact.getContactData()->points, [this](auto point) {
        return point != Vec2::ZERO && this->sceneFrame().containsPoint(point);
    });
}
