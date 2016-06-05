#include "pulse/scenes/SplashScene.hpp"
#include "pulse/ui/Resources.hpp"
#include "pulse/utilities/Geometry.hpp"
#include "pulse/utilities/Callback.hpp"
#include "SimpleAudioEngine.h"

using namespace pulse;
using namespace cocos2d;

SplashScene::SplashScene() {
    setBackground(LayerColor::create(Color4B::WHITE));

    SpriteFrameCache::getInstance()->addSpriteFramesWithFile(Resources::Spritesheets::Intro);
    AnimationCache::getInstance()->addAnimationsWithFile(Resources::Animations::Intro::Properties);
    image_ = Sprite::createWithSpriteFrameName(Resources::Images::Intro::Long);
    image_->retain();
    image_->setPosition(geometry::centerOf(sceneFrame()));
    image_->setScale(3);
    addChild(image_, 1);
}

SplashScene::~SplashScene() {
    AnimationCache::getInstance()->removeAnimation(Resources::Animations::Intro::Properties);
    SpriteFrameCache::getInstance()->removeSpriteFramesFromFile(Resources::Spritesheets::Intro);
    CC_SAFE_RELEASE(image_);
}

void SplashScene::onEnterTransitionDidFinish() {
    GameScene::onEnterTransitionDidFinish();

    const auto animation = AnimationCache::getInstance()->getAnimation(Resources::Animations::Intro::Long);
    const auto logoAnimation = Animate::create(animation);
    const auto logoAudio = CallFunc::create([]() {
        const auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
        audio->playEffect(Resources::Audio::Intro::Long, false, 1.0f, 1.0f, 1.0f);
    });
    const auto introAnimation = Spawn::createWithTwoActions(logoAnimation, logoAudio);
    const auto onCompletion = CallFunc::create([this]() {
        safe_callback(onSceneDismissed, this);
    });

    image_->runAction(Sequence::createWithTwoActions(introAnimation, onCompletion));
}
