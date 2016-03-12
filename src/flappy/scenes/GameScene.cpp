#include <flappy/scenes/GameScene.hpp>

USING_NS_CC;

bool GameScene::init() {
    if (!Layer::init()) {
        return false;
    }

    auto director = Director::getInstance();
    frame = Rect(director->getVisibleOrigin(), director->getVisibleSize());

    return true;
}
