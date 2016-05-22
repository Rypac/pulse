#include "pulse/AppDelegate.hpp"
#include "pulse/models/Resolution.hpp"
#include "pulse/scenes/DeveloperMenuScene.hpp"
#include "pulse/scenes/PulseGameScene.hpp"
#include "pulse/scenes/SplashScene.hpp"
#include "pulse/scenes/TitleScene.hpp"

using namespace cocos2d;
using namespace pulse;

AppDelegate::AppDelegate() {}

AppDelegate::~AppDelegate() {}

void AppDelegate::initGLContextAttrs() {
    GLContextAttrs glContextAttrs{8, 8, 8, 8, 24, 8};
    GLView::setGLContextAttrs(glContextAttrs);
}

bool AppDelegate::applicationDidFinishLaunching() {
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if (!glview) {
        glview = GLViewImpl::create("Pulse");
        director->setOpenGLView(glview);
    }

    director->setProjection(Director::Projection::_2D);
    director->setDisplayStats(false);
    director->setAnimationInterval(1.0f / 60);

    const auto& designResolution = Resolution::design();
    glview->setDesignResolutionSize(designResolution.size.width, designResolution.size.height, ResolutionPolicy::FIXED_HEIGHT);

    const auto displayResolution = Resolution::forFrame(glview->getFrameSize());
    director->setContentScaleFactor(displayResolution.contentScaleFactor());
    FileUtils::getInstance()->setSearchPaths(displayResolution.resourceSearchPaths());

    gameRunning(true);
    addSplashScene();

    return true;
}

void AppDelegate::applicationDidEnterBackground() {
    gameRunning(false);
    Director::getInstance()->stopAnimation();
}

void AppDelegate::applicationWillEnterForeground() {
    gameRunning(true);
    Director::getInstance()->startAnimation();
}

void AppDelegate::gameRunning(bool running) {
    Device::setKeepScreenOn(running);
    Device::setAccelerometerEnabled(running);
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
    Director::getInstance()->replaceScene(PhysicsScene::createScene(gameScene));
}

void AppDelegate::addDeveloperMenuScene() {
    const auto developerScene = DeveloperMenuScene::create(options);
    developerScene->onSceneDismissed = [](auto scene) {
        Director::getInstance()->popScene();
    };
    Director::getInstance()->pushScene(GameScene::createScene(developerScene));
}
