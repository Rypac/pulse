#include "pulse/scenes/SplashScene.hpp"
#include "pulse/utilities/Geometry.hpp"
#include "pulse/ui/Resources.hpp"
#include "SimpleAudioEngine.h"

using namespace pulse;
using namespace cocos2d;

SplashScene* SplashScene::create() {
    const auto scene = new (std::nothrow) SplashScene();
    if (scene && scene->init()) {
        scene->autorelease();
        return scene;
    }
    delete scene;
    return nullptr;
}

SplashScene::~SplashScene() {
    AnimationCache::getInstance()->removeAnimation(Resources::Animations::Intro::Properties);
    SpriteFrameCache::getInstance()->removeSpriteFramesFromFile(Resources::Spritesheets::Intro);
    CC_SAFE_RELEASE(image);
}

bool SplashScene::init() {
    if (!GameScene::init()) {
        return false;
    }

    const auto background = LayerColor::create(Color4B::WHITE);
    addChild(background);

    SpriteFrameCache::getInstance()->addSpriteFramesWithFile(Resources::Spritesheets::Intro);
    AnimationCache::getInstance()->addAnimationsWithFile(Resources::Animations::Intro::Properties);
    image = Sprite::createWithSpriteFrameName(Resources::Images::Intro::Long);
    image->retain();
    image->setPosition(geometry::centerOf(sceneFrame()));
    image->setScale(3, 3);
    addChild(image, 1);

    return true;
}

void SplashScene::onEnter() {
    GameScene::onEnter();
    image->runAction(logoAnimation());
}

Action* SplashScene::logoAnimation() {
    const auto animation = AnimationCache::getInstance()->getAnimation(Resources::Animations::Intro::Long);
    const auto logoAnimation = Animate::create(animation);
    const auto logoAudio = CallFunc::create([this]() {
        const auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
        audio->playEffect(Resources::Audio::Intro::Long, false, 1.0f, 1.0f, 1.0f);
    });
    const auto introAnimation = Spawn::createWithTwoActions(logoAnimation, logoAudio);
    const auto onCompletion = CallFunc::create([this]() {
        onSceneDismissed(this);
    });
    return Sequence::create(introAnimation, onCompletion, nullptr);
}
