#include "pulse/AppDelegate.hpp"
#include "pulse/scenes/DeveloperMenuScene.hpp"
#include "pulse/scenes/PulseGameScene.hpp"
#include "pulse/scenes/SplashScene.hpp"
#include "pulse/scenes/TitleScene.hpp"

using namespace cocos2d;
using namespace pulse;

struct Resolution {
    Size size;
    std::string path;
};

namespace DisplayResolution {
    static const auto Small = Resolution{Size{960, 540}, "small"};
    static const auto Medium = Resolution{Size{1280, 720}, "medium"};
    static const auto Large = Resolution{Size{1920, 1080}, "large"};
    static const auto ExtraLarge = Resolution{Size{2560, 1440}, "extra_large"};
}

AppDelegate::AppDelegate() {}

AppDelegate::~AppDelegate() {}

void AppDelegate::initGLContextAttrs() {
    GLContextAttrs glContextAttrs{8, 8, 8, 8, 24, 8};
    GLView::setGLContextAttrs(glContextAttrs);
}

Resolution displayResolutionForFrame(Size frame) {
    if (frame.height > DisplayResolution::Large.size.height) {
        return DisplayResolution::ExtraLarge;
    } else if (frame.height > DisplayResolution::Medium.size.height) {
        return DisplayResolution::Large;
    } else if (frame.height > DisplayResolution::Small.size.height) {
        return DisplayResolution::Medium;
    } else {
        return DisplayResolution::Small;
    }
}

float contentScaleFactor(Resolution resolution, Resolution designResolution) {
    const auto scaledHeight = resolution.size.height / designResolution.size.height;
    const auto scaledWidth = resolution.size.width / designResolution.size.width;
    return std::min(scaledHeight, scaledWidth);
}

std::vector<std::string> imageSearchPaths(Resolution resolution) {
    return {"images/" + resolution.path};
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

    const auto designResolution = DisplayResolution::Medium;
    glview->setDesignResolutionSize(designResolution.size.width, designResolution.size.height, ResolutionPolicy::FIXED_HEIGHT);

    const auto displayResolution = displayResolutionForFrame(glview->getFrameSize());
    director->setContentScaleFactor(contentScaleFactor(displayResolution, designResolution));
    FileUtils::getInstance()->setSearchPaths(imageSearchPaths(displayResolution));

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
