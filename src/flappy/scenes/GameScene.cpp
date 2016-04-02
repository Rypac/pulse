#include "flappy/scenes/GameScene.hpp"

using namespace cocos2d;
using namespace flappy;

Scene* GameScene::createScene(GameScene* gameScene) {
    const auto scene = Scene::create();
    scene->addChild(gameScene);
    return scene;
}

Scene* GameScene::createPhysicsScene(GameScene* gameScene, cocos2d::Vec2 gravity, bool autoStep) {
    const auto scene = Scene::createWithPhysics();
    scene->getPhysicsWorld()->setGravity(gravity);
    scene->getPhysicsWorld()->setAutoStep(autoStep);
    scene->addChild(gameScene);
    return scene;
}

bool GameScene::init() {
    if (!Layer::init()) {
        return false;
    }

    auto director = Director::getInstance();
    frame = Rect{director->getVisibleOrigin(), director->getVisibleSize()};
    status = Status::Initialising;

    return true;
}

void GameScene::initScene() {
    resumeScene();
}

void GameScene::clearScene() {
    stopScene();
}

void GameScene::resetScene() {
    clearScene();
    initScene();
}

void GameScene::pauseScene() {
    status = Status::Paused;
    Director::getInstance()->pause();
}

void GameScene::resumeScene() {
    status = Status::Running;
    Director::getInstance()->resume();
}

void GameScene::stopScene() {
    pauseScene();
    status = Status::Stopped;
}

void GameScene::exitScene() {
    stopScene();
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

bool GameScene::residesInSceneBounds(const Node& node) const {
    return node.getBoundingBox().intersectsRect(getFrame());
}
