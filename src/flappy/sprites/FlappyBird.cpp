#include <flappy/sprites/FlappyBird.hpp>

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
