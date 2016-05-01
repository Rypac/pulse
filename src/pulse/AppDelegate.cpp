#include "pulse/AppDelegate.hpp"
#include "pulse/scenes/DeveloperMenuScene.hpp"
#include "pulse/scenes/PulseGameScene.hpp"
#include "pulse/scenes/SplashScene.hpp"
#include "pulse/scenes/TitleScene.hpp"

#include <algorithm>

using namespace cocos2d;
using namespace pulse;

namespace DisplayResolution {
    static const auto Small = Size{960, 540};
    static const auto Medium = Size{1280, 720};
    static const auto Large = Size{1920, 1080};
    static const auto ExtraLarge = Size{2560, 1440};
    static const auto Design = Medium;
}

AppDelegate::AppDelegate() {}

AppDelegate::~AppDelegate() {}

void AppDelegate::initGLContextAttrs() {
    GLContextAttrs glContextAttrs{8, 8, 8, 8, 24, 8};
    GLView::setGLContextAttrs(glContextAttrs);
}

Size displayResolutionForFrame(Size frame) {
    if (frame.height > DisplayResolution::Large.height) {
        return DisplayResolution::ExtraLarge;
    } else if (frame.height > DisplayResolution::Medium.height) {
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
        glview = GLViewImpl::create("Pulse");
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
    splashScene->onSceneDismissed = [this](auto scene) {
        this->addTitleScene();
    };
    Director::getInstance()->runWithScene(GameScene::createScene(splashScene));
}

void AppDelegate::addTitleScene() {
    const auto titleScene = TitleScene::create();
    titleScene->onSceneDismissed = [this](auto scene) {
        this->addGameScene();
    };
    Director::getInstance()->replaceScene(GameScene::createScene(titleScene));
}

void AppDelegate::addGameScene() {
    const auto gameScene = PulseGameScene::create(options);
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
