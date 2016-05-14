#include "pulse/scenes/SplashScene.hpp"
#include "pulse/utilities/geometry.hpp"
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

bool SplashScene::init() {
    if (!GameScene::init()) {
        return false;
    }

    const auto background = LayerColor::create(Color4B::WHITE);
    addChild(background);

    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("animations/splash/tandem_intro_spritesheet.plist");
    image = Sprite::createWithSpriteFrameName("tandem_intro_long.png");
    image->setPosition(geometry::centerOf(sceneFrame()));
    image->setScale(3.6, 3.6);
    addChild(image, 1);

    return true;
}

void SplashScene::onEnter() {
    GameScene::onEnter();
    image->runAction(logoAnimation());
}

void SplashScene::onExit() {
    GameScene::onExit();
    SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("animations/splash/tandem_intro_spritesheet.plist");
}

Action* SplashScene::logoAnimation() {
    const auto cache = AnimationCache::getInstance();
    cache->addAnimationsWithFile("animations/splash/tandem_intro.plist");
    const auto animation = cache->getAnimation("tandem_intro_long");
    const auto logo = Animate::create(animation);
    const auto logoAudio = CallFunc::create([this]() {
        const auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
        audio->playEffect("audio/splash/tandem_intro_long.wav", false, 1.0f, 1.0f, 1.0f);
    });
    const auto introAnimation = Spawn::createWithTwoActions(logo, logoAudio);
    const auto onCompletion = CallFunc::create([this]() {
        onSceneDismissed(this);
    });
    return Sequence::create(introAnimation, onCompletion, nullptr);
}
