#include "flappy/scenes/GameScene.hpp"

using namespace cocos2d;
using namespace flappy;

bool GameScene::init() {
    if (!Layer::init()) {
        return false;
    }

    auto director = Director::getInstance();
    frame = Rect(director->getVisibleOrigin(), director->getVisibleSize());

    return true;
}

void GameScene::resetScene() {
    clearScene();
    initScene();
    if (isScenePaused()) {
        resumeScene();
    }
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

void GameScene::exitScene() {
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
