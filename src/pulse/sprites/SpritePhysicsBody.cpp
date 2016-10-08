#include "pulse/sprites/SpritePhysicsBody.hpp"

namespace pulse {
namespace physics_body {

using cocos2d::PhysicsBody;
using cocos2d::PhysicsContact;
using cocos2d::Size;
using cocos2d::Node;

enum class SpriteTag : int {
    None = 0,
    Hero = (1 << 0),
    Obstacle = (1 << 1),
    Path = (1 << 2),
    UI = (1 << 3),
};

static inline int asBitmask(SpriteTag tag) {
    return static_cast<int>(tag);
}

static inline bool isSprite(const PhysicsBody& body, SpriteTag tag) {
    return body.getCategoryBitmask() == asBitmask(tag);
}

static PhysicsBody* create(SpriteTag tag, const Size& size) {
    const auto body = PhysicsBody::createBox(size);
    body->setDynamic(false);
    body->setGravityEnable(false);
    body->setCategoryBitmask(asBitmask(tag));
    body->setCollisionBitmask(asBitmask(SpriteTag::None));
    body->setContactTestBitmask(asBitmask(SpriteTag::None));
    return body;
}

PhysicsBody* createHero(const Size& size) {
    const auto body = create(SpriteTag::Hero, size);
    body->setCollisionBitmask(asBitmask(SpriteTag::Obstacle));
    body->setContactTestBitmask(
        asBitmask(SpriteTag::Obstacle) |
        asBitmask(SpriteTag::Path) |
        asBitmask(SpriteTag::UI));
    return body;
}

PhysicsBody* createObstacle(const Size& size) {
    const auto body = create(SpriteTag::Obstacle, size);
    body->setCollisionBitmask(asBitmask(SpriteTag::Hero));
    body->setContactTestBitmask(asBitmask(SpriteTag::Hero));
    return body;
}

PhysicsBody* createPath(const Size& size) {
    const auto body = create(SpriteTag::Path, size);
    body->setContactTestBitmask(asBitmask(SpriteTag::Hero));
    return body;
}

PhysicsBody* createUI(const Size& size) {
    const auto body = create(SpriteTag::UI, size);
    body->setContactTestBitmask(asBitmask(SpriteTag::Hero));
    return body;
}

PhysicsBody* clone(PhysicsBody* body, const Size& size) {
    const auto clone = PhysicsBody::createBox(size);
    clone->setDynamic(body->isDynamic());
    clone->setGravityEnable(body->isGravityEnabled());
    clone->setCategoryBitmask(body->getCategoryBitmask());
    clone->setCollisionBitmask(body->getCollisionBitmask());
    clone->setContactTestBitmask(body->getContactTestBitmask());
    return clone;
}

bool isHero(const PhysicsBody& body) {
    return isSprite(body, SpriteTag::Hero);
}

bool isObstacle(const PhysicsBody& body) {
    return isSprite(body, SpriteTag::Obstacle);
}

bool isPath(const PhysicsBody& body) {
    return isSprite(body, SpriteTag::Path);
}

bool isUI(const PhysicsBody& body) {
    return isSprite(body, SpriteTag::UI);
}

void stopCollisions(PhysicsBody* body) {
    body->setCollisionBitmask(asBitmask(SpriteTag::None));
    body->setContactTestBitmask(asBitmask(SpriteTag::None));
}

std::optional<Node*> nodeInContact(const PhysicsBody& body1, const PhysicsBody& body2, NodePredicate isNode) {
    if (isNode(body1)) {
        return std::make_optional(body1.getNode());
    }
    return isNode(body2) ? std::make_optional(body2.getNode()) : std::nullopt;
}

std::optional<Node*> nodeInContact(const PhysicsContact& contact, const NodePredicate isNode) {
    return nodeInContact(*contact.getShapeA()->getBody(), *contact.getShapeB()->getBody(), isNode);
}

namespace collision {

bool heroAndObstacle(const PhysicsContact& contact) {
    const auto& body1 = *contact.getShapeA()->getBody();
    const auto& body2 = *contact.getShapeB()->getBody();
    return (isHero(body1) or isHero(body2)) and (isObstacle(body1) or isObstacle(body2));
}

bool heroAndPath(const PhysicsContact& contact) {
    const auto& body1 = *contact.getShapeA()->getBody();
    const auto& body2 = *contact.getShapeB()->getBody();
    return (isHero(body1) or isHero(body2)) and (isPath(body1) or isPath(body2));
}

bool withUI(const PhysicsContact& contact) {
    const auto& body1 = *contact.getShapeA()->getBody();
    const auto& body2 = *contact.getShapeB()->getBody();
    return isUI(body1) or isUI(body2);
}

}  // collision

}  // physics_body
}  // pulse
