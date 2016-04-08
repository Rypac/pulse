#include "pulse/scenes/SplashScene.hpp"
#include "pulse/utilities/geometry.hpp"

using pulse::SplashScene;
using cocos2d::Size;
using cocos2d::LayerColor;
using cocos2d::Color4B;
using cocos2d::experimental::ui::VideoPlayer;

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

    videoPlayer = VideoPlayer::create();
    videoPlayer->setFileName("tandem_logo.mp4");
    videoPlayer->setPosition(geometry::centerOf(frame));
    videoPlayer->setContentSize(Size{320, 320});
    videoPlayer->addEventListener([this](auto ref, auto eventType) {
        if (eventType == VideoPlayer::EventType::COMPLETED ||
            eventType == VideoPlayer::EventType::PAUSED ||
            eventType == VideoPlayer::EventType::STOPPED) {
            if (onSceneDismissed) {
                onSceneDismissed(this);
            }
        }
    });
    addChild(videoPlayer, 1);

    return true;
}

void SplashScene::onEnter() {
    GameScene::onEnter();
    videoPlayer->play();
}
