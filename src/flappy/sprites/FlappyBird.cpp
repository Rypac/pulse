#include "flappy/sprites/FlappyBird.hpp"
#include "flappy/utilities/Physics.hpp"

using namespace cocos2d;
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
    const auto verticalDistance = velocity.y * dt;
    setPositionY(getPositionY() + verticalDistance);
}
