#include "pulse/AppDelegate.hpp"
#include "pulse/models/Resolution.hpp"
#include "pulse/scenes/DeveloperMenuScene.hpp"
#include "pulse/scenes/InGameMenuScene.hpp"
#include "pulse/scenes/PulseGameScene.hpp"
#include "pulse/scenes/SplashScene.hpp"
#include "pulse/scenes/TitleScene.hpp"

using namespace cocos2d;
using namespace pulse;

AppDelegate::AppDelegate() {}

AppDelegate::~AppDelegate() {
    CC_SAFE_RELEASE(gameScene);
    CC_SAFE_RELEASE(titleScene);
}

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
    titleScene = TitleScene::create();
    titleScene->retain();
    titleScene->onSceneDismissed = [this](auto scene) {
        this->addGameScene();
        CC_SAFE_RELEASE_NULL(titleScene);
    };
    Director::getInstance()->replaceScene(GameScene::createScene(titleScene));
}

void AppDelegate::addGameScene() {
    gameScene = PulseGameScene::create(options);
    gameScene->retain();
    gameScene->setBackground(titleScene->background());
    gameScene->onEnterMenu = [this](auto scene) {
        this->addInGameMenuScene();
    };
    gameScene->onSceneDismissed = [this](auto scene) {
        this->addTitleScene();
        CC_SAFE_RELEASE_NULL(gameScene);
    };
    Director::getInstance()->replaceScene(PhysicsScene::createScene(gameScene));
}

void AppDelegate::addInGameMenuScene() {
    const auto menuScene = InGameMenuScene::create();
    menuScene->onResumeGame = [](auto scene) {
        Director::getInstance()->popScene();
    };
    menuScene->onRestartGame = [this](auto scene) {
        gameScene->resetScene();
        Director::getInstance()->popScene();
    };
    menuScene->onQuitGame = [this](auto scene) {
        Director::getInstance()->popScene();
        gameScene->onSceneDismissed(gameScene);
    };
    Director::getInstance()->pushScene(GameScene::createScene(menuScene));
}

void AppDelegate::addDeveloperMenuScene() {
    const auto developerScene = DeveloperMenuScene::create(options);
    developerScene->onSceneDismissed = [](auto scene) {
        Director::getInstance()->popScene();
    };
    Director::getInstance()->pushScene(GameScene::createScene(developerScene));
}
