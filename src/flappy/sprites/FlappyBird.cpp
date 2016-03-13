#include "flappy/sprites/FlappyBird.hpp"
#include "flappy/utilities/Physics.hpp"

USING_NS_CC;
using namespace flappy;

bool FlappyBird::init() {
    if (!Sprite::init()) {
        return false;
    }

    setTextureRect(Rect(0, 0, 30, 30));
    setColor(Color3B::WHITE);

    return true;
}

void FlappyBird::update(float dt) {
    velocity.y += (physics::Gravity.y * dt);
    velocity.y *= linearDamping.y;
    if (velocity.y < 0) {
        velocity.y = 0;
    }

    auto upwardDistance = velocity.y * dt;
    auto downwardDistance = physics::Gravity.y * dt;
    setPositionY(getPositionY() + upwardDistance + downwardDistance);
}
