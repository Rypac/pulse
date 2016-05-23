#include "pulse/ui/Button.hpp"

using pulse::ui::Button;

static const auto AnimationSpeed = 0.04;

Button* Button::create() {
    const auto button = new (std::nothrow) Button();
    if (button && button->init()) {
        button->autorelease();
        return button;
    }
    delete button;
    return nullptr;
}

Button* Button::create(const std::string& filename) {
    const auto button = new (std::nothrow) Button();
    if (button && button->initWithFile(filename)) {
        button->autorelease();
        return button;
    }
    delete button;
    return nullptr;
}

bool Button::initWithTexture(cocos2d::Texture2D* texture, const cocos2d::Rect& rect, bool rotated) {
    if (!Sprite::initWithTexture(texture, rect, rotated)) {
        return false;
    }

    listener_ = cocos2d::EventListenerTouchOneByOne::create();
    listener_->retain();
    listener_->onTouchBegan = CC_CALLBACK_2(Button::onTouchDidBegin, this);
    listener_->onTouchMoved = CC_CALLBACK_2(Button::onTouchDidMove, this);
    listener_->onTouchEnded = CC_CALLBACK_2(Button::onTouchDidEnd, this);
    getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener_, this);

    return true;
}

Button::~Button() {
    if (listener_) {
        getEventDispatcher()->removeEventListener(listener_);
        listener_->release();
    }
}

bool Button::onTouchDidBegin(cocos2d::Touch* touch, cocos2d::Event* event) {
    if (not getBoundingBox().containsPoint(touch->getLocation())) {
        return false;
    }

    if (reactsToTouch_) {
        savedScale_ = getScale();
        runAction(cocos2d::ScaleTo::create(AnimationSpeed, savedScale_ + touchScale()));
    }
    if (onTouchBegan) {
        onTouchBegan(this);
    }
    return true;
}

void Button::onTouchDidMove(cocos2d::Touch* touch, cocos2d::Event* event) {
    if (cancelled_ or getBoundingBox().containsPoint(touch->getLocation())) {
        return;
    } else {
        cancelled_ = true;
    }

    if (reactsToTouch_) {
        runAction(cocos2d::ScaleTo::create(AnimationSpeed, savedScale_));
    }
    if (onTouchCancelled) {
        onTouchCancelled(this);
    }
}

void Button::onTouchDidEnd(cocos2d::Touch* touch, cocos2d::Event* event) {
    if (cancelled_) {
        cancelled_ = false;
        return;
    }

    if (reactsToTouch_) {
        runAction(cocos2d::ScaleTo::create(AnimationSpeed, savedScale_));
    }
    if (onTouchEnded) {
        onTouchEnded(this);
    }
}