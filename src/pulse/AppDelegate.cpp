#include "pulse/AppDelegate.hpp"
#include "pulse/extensions/Ref.hpp"
#include "pulse/models/Resolution.hpp"
#include "pulse/scenes/DeveloperMenuScene.hpp"
#include "pulse/scenes/GameOverScene.hpp"
#include "pulse/scenes/PauseMenuScene.hpp"
#include "pulse/scenes/ModeSelectionScene.hpp"
#include "pulse/scenes/PulseGameScene.hpp"
#include "pulse/scenes/SplashScene.hpp"
#include "pulse/scenes/TitleScene.hpp"
#include "pulse/ui/Resources.hpp"

using namespace cocos2d;
using namespace pulse;

AppDelegate::AppDelegate() {}

AppDelegate::~AppDelegate() {
    CC_SAFE_RELEASE(gameScene);
    CC_SAFE_RELEASE(titleScene);
    CC_SAFE_RELEASE(animatedBackground_);
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
    allocatedResources();
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

void AppDelegate::allocatedResources() {
    animatedBackground_ = ParticleSystemQuad::create(Resources::Particles::AmbientBackground);
    animatedBackground_->retain();
}

Node* AppDelegate::sharedAnimatedBackground() const {
    return animatedBackground_;
}

void AppDelegate::addSplashScene() {
    const auto splashScene = autoreleased<SplashScene>();
    splashScene->onSceneDismissed = [this](auto scene) {
        this->addTitleScene();
    };
    Director::getInstance()->runWithScene(splashScene);
}

void AppDelegate::addTitleScene() {
    titleScene = retained<TitleScene>();
    titleScene->setBackground(sharedAnimatedBackground());
    titleScene->onPlaySelected = [this](auto scene) {
        this->addGameScene();
        CC_SAFE_RELEASE_NULL(titleScene);
    };
    titleScene->onModesSelected = [this](auto scene) {
        const auto modeSelectionScene = autoreleased<ModeSelectionScene>(options.gameMode);
        modeSelectionScene->onSceneDismissed = [this](auto scene) {
            options.gameMode = scene->selectedMode();
            scene->removeFromParent();
        };
        scene->addChild(modeSelectionScene, 10);
    };
    titleScene->onSettingsSelected = [this](auto scene) {
        const auto developerScene = autoreleased<DeveloperMenuScene>(options);
        developerScene->onSceneDismissed = [](auto scene) {
            scene->removeFromParent();
        };
        scene->addChild(developerScene, 10);
    };
    Director::getInstance()->replaceScene(titleScene);
}

void AppDelegate::addGameScene() {
    gameScene = retained<PulseGameScene>(options);
    gameScene->setBackground(sharedAnimatedBackground());
    gameScene->onEnterMenu = [this](auto scene) {
        this->addPauseMenuScene();
    };
    gameScene->onGameOver = [this](auto scene) {
        this->addGameOverScene();
        CC_SAFE_RELEASE_NULL(gameScene);
    };
    gameScene->onSceneDismissed = [this](auto scene) {
        this->addTitleScene();
        CC_SAFE_RELEASE_NULL(gameScene);
    };
    gameScene->startNewGame();
    Director::getInstance()->replaceScene(gameScene);
}

void AppDelegate::addPauseMenuScene() {
    const auto menuScene = autoreleased<PauseMenuScene>();
    menuScene->onResumeGame = [](auto scene) {
        Director::getInstance()->popScene();
    };
    menuScene->onRestartGame = [this](auto scene) {
        gameScene->startNewGame();
        Director::getInstance()->popScene();
    };
    menuScene->onQuitGame = [this](auto scene) {
        Director::getInstance()->popScene();
        gameScene->onSceneDismissed(gameScene);
    };
    Director::getInstance()->pushScene(menuScene);
}

void AppDelegate::addGameOverScene() {
    const auto scene = autoreleased<GameOverScene>();
    scene->setBackground(sharedAnimatedBackground());
    scene->onRestartGame = [this](auto scene) {
        this->addGameScene();
    };
    Director::getInstance()->replaceScene(scene);
}
