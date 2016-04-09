#include "pulse/sprites/Pulse.hpp"

using namespace cocos2d;
using namespace pulse;

Pulse* Pulse::create() {
    Pulse *pulse = new (std::nothrow) Pulse();
    if (pulse && pulse->init()) {
        pulse->autorelease();
        return pulse;
    }
    delete pulse;
    return nullptr;
}

bool Pulse::init() {
    if (!Sprite::init()) {
        return false;
    }

    setColor(Color3B::WHITE);

    return true;
}

Pulse* Pulse::create(Size size) {
    return Pulse::create(Rect{Vec2{0, 0}, size});
}

Pulse* Pulse::create(Rect frame) {
    const auto pulse = Pulse::create();
    pulse->setPosition(frame.origin);
    pulse->setContentSize(frame.size);
    pulse->setTextureRect(frame);
    return pulse;
}
