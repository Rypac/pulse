#include "pulse/scenes/GameScene.hpp"

using namespace cocos2d;
using namespace pulse;

GameScene::GameScene(): frame_{Rect::ZERO}, background_{nullptr} {}

GameScene::~GameScene() {
    CC_SAFE_RELEASE(background_);
}

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
    frame_ = Rect{director->getVisibleOrigin(), director->getVisibleSize()};

    return true;
}

Node* GameScene::background() const {
    return background_;
}

void GameScene::removeBackground() {
    if (background_) {
        removeChild(background_);
        CC_SAFE_RELEASE_NULL(background_);
    }
}

void GameScene::setBackground(Node *background) {
    removeBackground();
    background->removeFromParent();

    background_ = background;
    background_->retain();
    addChild(background_);
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
