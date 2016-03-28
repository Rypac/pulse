#include "flappy/sprites/FlappyBird.hpp"
#include "flappy/sprites/SpriteTag.hpp"

using namespace cocos2d;
using namespace flappy;

bool FlappyBird::init() {
    if (!Sprite::init()) {
        return false;
    }

    setColor(Color3B::WHITE);

    return true;
}

FlappyBird* FlappyBird::create(Size size) {
    return FlappyBird::create(Rect{Vec2{0, 0}, size});
}

FlappyBird* FlappyBird::create(Rect frame) {
    const auto flappyBird = FlappyBird::create();
    flappyBird->setPosition(frame.origin);
    flappyBird->setContentSize(frame.size);
    flappyBird->setTextureRect(frame);

    const auto physicsBody = PhysicsBody::createBox(frame.size);
    physicsBody->setDynamic(true);
    physicsBody->setGravityEnable(false);
    physicsBody->setCategoryBitmask(static_cast<int>(SpriteTag::Hero));
    physicsBody->setCollisionBitmask(static_cast<int>(SpriteTag::None));
    physicsBody->setContactTestBitmask(static_cast<int>(SpriteTag::Obstacle));
    flappyBird->setPhysicsBody(physicsBody);
    return flappyBird;
}
