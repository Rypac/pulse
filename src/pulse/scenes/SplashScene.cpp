#include "pulse/scenes/SplashScene.hpp"
#include "pulse/utilities/geometry.hpp"

using pulse::SplashScene;
using cocos2d::Color4B;
using cocos2d::LayerColor;
using cocos2d::Size;
using cocos2d::Sprite;

SplashScene* SplashScene::create() {
    SplashScene *scene = new (std::nothrow) SplashScene();
    if (scene && scene->init()) {
        scene->autorelease();
        return scene;
    }
    CC_SAFE_DELETE(scene);
    return nullptr;
}

bool SplashScene::init() {
    if (!GameScene::init()) {
        return false;
    }

    const auto background = LayerColor::create(Color4B::WHITE);
    addChild(background);

    image = Sprite::create("images/splash/normal.png");
    image->setPosition(geometry::centerOf(frame));
    addChild(image, 1);

    return true;
}

void SplashScene::onEnter() {
    GameScene::onEnter();
}
