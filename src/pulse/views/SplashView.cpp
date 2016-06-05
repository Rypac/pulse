#include "pulse/views/SplashView.hpp"
#include "pulse/extensions/Rect.hpp"
#include "pulse/ui/Resources.hpp"

using namespace pulse;
using namespace cocos2d;

SplashView::SplashView() {
    setBackground(LayerColor::create(Color4B::WHITE));

    SpriteFrameCache::getInstance()->addSpriteFramesWithFile(Resources::Spritesheets::Intro);
    AnimationCache::getInstance()->addAnimationsWithFile(Resources::Animations::Intro::Properties);
    image_ = Sprite::createWithSpriteFrameName(Resources::Images::Intro::Long);
    image_->retain();
    image_->setPosition(rect::center(frame()));
    image_->setScale(2);
    addChild(image_, 1);
}

SplashView::~SplashView() {
    AnimationCache::getInstance()->removeAnimation(Resources::Animations::Intro::Properties);
    SpriteFrameCache::getInstance()->removeSpriteFramesFromFile(Resources::Spritesheets::Intro);
    CC_SAFE_RELEASE(image_);
}

Sprite* SplashView::image() const {
    return image_;
}

Animation* SplashView::animation() const {
    return AnimationCache::getInstance()->getAnimation(Resources::Animations::Intro::Long);
}
