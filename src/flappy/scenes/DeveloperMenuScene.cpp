#include "flappy/scenes/DeveloperMenuScene.hpp"

using namespace cocos2d;
using namespace flappy;

bool DeveloperMenuScene::init() {
    if (!GameScene::init()) {
        return false;
    }

    return true;
}
