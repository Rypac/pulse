#include "flappy/sprites/FlappyBird.hpp"

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
    return flappyBird;
}
