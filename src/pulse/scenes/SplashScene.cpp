#include "pulse/scenes/SplashScene.hpp"
#include "pulse/utilities/geometry.hpp"
#include "SimpleAudioEngine.h"

using pulse::SplashScene;
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

    image = Sprite::create("splash/normal.png");
    image->setPosition(geometry::centerOf(sceneFrame()));
    addChild(image, 1);

    return true;
}

void SplashScene::onEnter() {
    GameScene::onEnter();
    runAction(splashScreenAnimation());
}

Action* SplashScene::splashScreenAnimation() {
    const auto normalImage = CallFunc::create([this]() {
        image->setTexture("splash/normal.png");
    });
    const auto winkImage = CallFunc::create([this]() {
        image->setTexture("splash/wink.png");
    });
    const auto winkSound = CallFunc::create([this]() {
        const auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
        audio->playEffect("audio/splash/wink.wav", false, 1.0f, 1.0f, 1.0f);
    });
    const auto finish = CallFunc::create([this]() {
        onSceneDismissed(this);
    });

    return Sequence::create(
        DelayTime::create(1.0),
        Spawn::createWithTwoActions(winkImage, winkSound),
        DelayTime::create(0.5),
        normalImage,
        DelayTime::create(1),
        finish,
        nullptr
    );
}
