#include "pulse/scenes/BaseScene.hpp"

using namespace cocos2d;
using namespace pulse;

BaseScene::BaseScene(): background_{nullptr} {
    const auto director = Director::getInstance();
    frame_ = Rect{director->getVisibleOrigin(), director->getVisibleSize()};
    init();
}

BaseScene::~BaseScene() {
    CC_SAFE_RELEASE(background_);
}

void BaseScene::removeBackground() {
    if (background_) {
        removeChild(background_);
        CC_SAFE_RELEASE_NULL(background_);
    }
}

void BaseScene::setBackground(Node *background) {
    removeBackground();
    background_ = background;
    background_->retain();
    background_->removeFromParent();
    addChild(background_, -1);
}

void BaseScene::resumeScene() {
    Director::getInstance()->resume();
}

void BaseScene::pauseScene() {
    Director::getInstance()->pause();
}

void BaseScene::quit() {
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

bool BaseScene::residesInSceneBounds(const Node& node) const {
    return node.getBoundingBox().intersectsRect(sceneFrame());
}
