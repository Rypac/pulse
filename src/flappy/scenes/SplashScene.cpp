#include "flappy/scenes/SplashScene.hpp"
#include "flappy/utilities/geometry.hpp"

using flappy::SplashScene;
using cocos2d::Scene;
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

    videoPlayer = VideoPlayer::create();
    videoPlayer->setPosition(geometry::centerOf(frame));
    videoPlayer->setFileName("TandemLogo.mp4");
    videoPlayer->setFullScreenEnabled(true);
    videoPlayer->addEventListener([this](auto ref, auto eventType) {
        if (eventType == VideoPlayer::EventType::COMPLETED) {
            if (onSceneDismissed) {
                onSceneDismissed(this);
            }
        }
    });
    addChild(videoPlayer);

    return true;
}

void SplashScene::onEnter() {
    GameScene::onEnter();
    videoPlayer->play();
}
