#include "flappy/sprites/SpritePhysicsBody.hpp"

namespace flappy {
namespace physics_body {

using cocos2d::PhysicsBody;
using cocos2d::PhysicsContact;
using cocos2d::Size;
using cocos2d::Node;

enum class SpriteTag : int {
    None = 0,
    Hero = (1 << 0),
    Obstacle = (1 << 1),
    Path = (2 << 1)
};

static inline int asBitmask(SpriteTag tag) {
    return static_cast<int>(tag);
}

static inline bool isSprite(const PhysicsBody& body, SpriteTag tag) {
    return body.getCategoryBitmask() == asBitmask(tag);
}

static PhysicsBody* create(SpriteTag tag, Size size) {
    const auto body = PhysicsBody::createBox(size);
    body->setDynamic(true);
    body->setGravityEnable(false);
    body->setCategoryBitmask(asBitmask(tag));
    body->setCollisionBitmask(asBitmask(SpriteTag::None));
    return body;
}

PhysicsBody* createHero(Size size) {
    const auto body = create(SpriteTag::Hero, size);
    body->setContactTestBitmask(asBitmask(SpriteTag::Obstacle) | asBitmask(SpriteTag::Path));
    return body;
}

PhysicsBody* createObstacle(Size size) {
    const auto body = create(SpriteTag::Obstacle, size);
    body->setContactTestBitmask(asBitmask(SpriteTag::Hero));
    return body;
}

PhysicsBody* createPath(Size size) {
    const auto body = create(SpriteTag::Path, size);
    body->setContactTestBitmask(asBitmask(SpriteTag::Hero));
    return body;
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

std::optional<Node*> nodeInContact(const PhysicsBody& body1, const PhysicsBody& body2, NodePredicate isNode) {
    return isNode(body1) ? std::make_optional(body1.getNode())
         : isNode(body2) ? std::make_optional(body2.getNode())
         : std::nullopt;
}

std::optional<Node*> nodeInContact(const PhysicsContact& contact, const NodePredicate isNode) {
    return nodeInContact(*contact.getShapeA()->getBody(), *contact.getShapeB()->getBody(), isNode);
}

namespace collision {

    bool heroAndObstacle(const PhysicsContact& contact) {
        return heroAndObstacle(*contact.getShapeA()->getBody(), *contact.getShapeB()->getBody());
    }

    bool heroAndObstacle(const PhysicsBody& body1, const PhysicsBody& body2) {
        return isHero(body1) || isHero(body2) ? isObstacle(body1) || isObstacle(body2) : false;
    }

    bool heroAndPath(const PhysicsContact& contact) {
        return heroAndPath(*contact.getShapeA()->getBody(), *contact.getShapeB()->getBody());
    }

    bool heroAndPath(const PhysicsBody& body1, const PhysicsBody& body2) {
        return isHero(body1) || isHero(body2) ? isPath(body1) || isPath(body2) : false;
    }

} } }
