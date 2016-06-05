#include "pulse/views/View.hpp"

View::View(): background_{nullptr} {
    init();
    const auto director = cocos2d::Director::getInstance();
    frame_ = cocos2d::Rect{director->getVisibleOrigin(), director->getVisibleSize()};
}

View::~View() {
    if (background_) {
        background_->release();
    }
}

void View::setBackground(Node *background) {
    removeBackground();
    background_ = background;
    background_->retain();
    background_->removeFromParent();
    addChild(background_);
}

void View::removeBackground() {
    if (background_) {
        removeChild(background_);
        background_->release();
        background_ = nullptr;
    }
}

bool View::residesInBounds(const cocos2d::Node& node) const {
    return node.getBoundingBox().intersectsRect(frame());
}
