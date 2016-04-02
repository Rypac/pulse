#include "flappy/AppDelegate.h"
#include "flappy/scenes/DeveloperMenuScene.hpp"
#include "flappy/scenes/FlappyBirdScene.hpp"
#include "flappy/scenes/SplashScene.hpp"

#include <algorithm>

using namespace cocos2d;
using namespace flappy;

namespace DisplayResolution {
    static const auto Design = Size{960, 640};
    static const auto Small = Size{960, 640};
    static const auto Medium = Size{1920, 1080};
    static const auto Large = Size{2560, 1440};
}

AppDelegate::AppDelegate() {}

AppDelegate::~AppDelegate() {}

void AppDelegate::initGLContextAttrs() {
    GLContextAttrs glContextAttrs{8, 8, 8, 8, 24, 8};
    GLView::setGLContextAttrs(glContextAttrs);
}

Size displayResolutionForFrame(Size frame) {
    if (frame.height > DisplayResolution::Medium.height) {
        return DisplayResolution::Large;
    } else if (frame.height > DisplayResolution::Small.height) {
        return DisplayResolution::Medium;
    } else {
        return DisplayResolution::Small;
    }
}

float contentScaleFactorForFrame(Size frame) {
    const auto resolution = displayResolutionForFrame(frame);
    const auto scaledHeight = resolution.height / DisplayResolution::Design.height;
    const auto scaledWidth = resolution.width / DisplayResolution::Design.width;
    return std::min(scaledHeight, scaledWidth);
}

bool AppDelegate::applicationDidFinishLaunching() {
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if (!glview) {
        glview = GLViewImpl::create("FlappyBird");
        director->setOpenGLView(glview);
    }

    director->setDisplayStats(false);
    director->setAnimationInterval(1.0 / 60);

    glview->setDesignResolutionSize(DisplayResolution::Design.width, DisplayResolution::Design.height, ResolutionPolicy::FIXED_HEIGHT);
    const auto frame = glview->getFrameSize();
    director->setContentScaleFactor(contentScaleFactorForFrame(frame));

    addSplashScene();

    return true;
}

void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();
}

void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();
}

void AppDelegate::addSplashScene() {
    const auto splashScene = SplashScene::create();
    splashScene->onSceneDismissed = [this](auto splashScene) {
        this->addGameScene();
    };
    Director::getInstance()->runWithScene(GameScene::createScene(splashScene));
}

void AppDelegate::addGameScene() {
    const auto gameScene = FlappyBirdScene::create(options);
    gameScene->onEnterMenu = [this](auto scene) {
        this->addDeveloperMenuScene();
    };
    Director::getInstance()->replaceScene(GameScene::createPhysicsScene(gameScene));
}

void AppDelegate::addDeveloperMenuScene() {
    const auto developerScene = DeveloperMenuScene::create(options);
    developerScene->onSceneDismissed = [](auto scene) {
        Director::getInstance()->popScene();
    };
    Director::getInstance()->pushScene(GameScene::createScene(developerScene));
}
