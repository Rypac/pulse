#include "pulse/sprites/Player.hpp"

using namespace pulse;

Player* Player::create() {
    const auto sprite = new (std::nothrow) Player();
    if (sprite && sprite->init()) {
        sprite->autorelease();
        return sprite;
    }
    delete sprite;
    return nullptr;
}

const cocos2d::Vec2& Player::velocity() const {
    return velocity_;
}

void Player::setVelocity(const cocos2d::Vec2& velocity) {
    velocity_ = velocity;
}

void Player::update(float dt) {
    setPosition(getPosition() + velocity() * dt);
}
