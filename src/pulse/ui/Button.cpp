#include "pulse/ui/Button.hpp"
#include "pulse/actions/CallbackAfter.hpp"
#include "pulse/extensions/Node.hpp"
#include "pulse/extensions/Ref.hpp"
#include "pulse/utilities/Callback.hpp"

using pulse::ui::Button;

static const auto AnimationSpeed = 0.04;

Button* Button::create(const std::string& filename) {
    const auto button = new (std::nothrow) Button();
    if (button and button->initWithSpriteFrameName(filename)) {
        button->autorelease();
        return button;
    }
    delete button;
    return nullptr;
}

bool Button::initWithTexture(cocos2d::Texture2D* texture, const cocos2d::Rect& rect, bool rotated) {
    if (not Sprite::initWithTexture(texture, rect, rotated)) {
        return false;
    }

    listener_ = cocos2d::EventListenerTouchOneByOne::create();
    listener_->retain();
    listener_->setSwallowTouches(true);
    listener_->onTouchBegan = member_callback(this, &Button::onTouchDidBegin);
    listener_->onTouchMoved = member_callback(this, &Button::onTouchDidMove);
    listener_->onTouchEnded = member_callback(this, &Button::onTouchDidEnd);
    getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener_, this);

    return true;
}

Button::Button()
: reactsToTouch_{true}
, touchScale_{-0.1f}
, savedScale_{1.0f}
, cancelled_{false}
, disableAfterClick_{false}
{}

Button::~Button() {
    if (listener_) {
        getEventDispatcher()->removeEventListener(listener_);
        listener_->release();
    }
}

bool Button::onTouchDidBegin(cocos2d::Touch* touch, cocos2d::Event* event) {
    if (not node::containsPoint(this, touch->getLocation())) {
        return false;
    }

    if (reactsToTouch_) {
        savedScale_ = getScale();
        runAction(cocos2d::ScaleTo::create(AnimationSpeed, savedScale_ + touchScale()));
    }
    safe_callback(onTouchBegan, this);
    return true;
}

void Button::onTouchDidMove(cocos2d::Touch* touch, cocos2d::Event* event) {
    if (cancelled_ or node::containsPoint(this, touch->getLocation())) {
        return;
    } else {
        cancelled_ = true;
    }

    if (reactsToTouch_) {
        runAction(autoreleased<CallbackAfter>(
            cocos2d::ScaleTo::create(AnimationSpeed, savedScale_),
            [this]() { safe_callback(onTouchCancelled, this); }
        ));
    } else {
        safe_callback(onTouchCancelled, this);
    }
}

void Button::onTouchDidEnd(cocos2d::Touch* touch, cocos2d::Event* event) {
    if (cancelled_) {
        cancelled_ = false;
        return;
    }

    if (reactsToTouch_) {
        runAction(autoreleased<CallbackAfter>(
            cocos2d::ScaleTo::create(AnimationSpeed, savedScale_),
            [this]() { safe_callback(onTouchEnded, this); }
        ));
    } else {
        safe_callback(onTouchEnded, this);
    }

    if (disableAfterClick_) {
        setTouchEnabled(false);
    }
}
