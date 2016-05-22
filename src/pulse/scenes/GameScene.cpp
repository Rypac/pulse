#include "pulse/scenes/GameScene.hpp"

using namespace cocos2d;
using namespace pulse;

Scene* GameScene::createScene(GameScene* gameScene) {
    const auto scene = Scene::create();
    scene->addChild(gameScene);
    return scene;
}

bool GameScene::init() {
    if (!Layer::init()) {
        return false;
    }

    const auto director = Director::getInstance();
    frame = Rect{director->getVisibleOrigin(), director->getVisibleSize()};

    return true;
}

void GameScene::resumeScene() {
    Director::getInstance()->resume();
}

void GameScene::pauseScene() {
    Director::getInstance()->pause();
}

void GameScene::quit() {
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

bool GameScene::residesInSceneBounds(const Node& node) const {
    return node.getBoundingBox().intersectsRect(sceneFrame());
}
