#include "flappy/scenes/GameScene.hpp"

USING_NS_CC;
using namespace flappy;

bool GameScene::init() {
    if (!Layer::init()) {
        return false;
    }

    auto director = Director::getInstance();
    frame = Rect(director->getVisibleOrigin(), director->getVisibleSize());

    return true;
}

bool GameScene::isScenePaused() {
    return Director::getInstance()->isPaused();
}

void GameScene::pauseScene() {
    Director::getInstance()->pause();
}

void GameScene::resumeScene() {
    Director::getInstance()->resume();
}
