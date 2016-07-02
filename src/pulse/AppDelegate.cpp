#include "pulse/AppDelegate.hpp"
#include "pulse/extensions/Ref.hpp"
#include "pulse/models/Resolution.hpp"
#include "pulse/scenes/DeveloperMenuScene.hpp"
#include "pulse/scenes/GameOverScene.hpp"
#include "pulse/scenes/PauseMenuScene.hpp"
#include "pulse/scenes/ModeSelectionScene.hpp"
#include "pulse/scenes/SplashScene.hpp"
#include "pulse/ui/Resources.hpp"

using namespace cocos2d;
using namespace pulse;

AppDelegate::AppDelegate() {}

AppDelegate::~AppDelegate() {
    CC_SAFE_RELEASE(gameScene_);
    CC_SAFE_RELEASE(titleScene_);
    CC_SAFE_RELEASE(animatedBackground_);
}

void AppDelegate::initGLContextAttrs() {
    GLContextAttrs glContextAttrs{8, 8, 8, 8, 24, 8};
    GLView::setGLContextAttrs(glContextAttrs);
}

bool AppDelegate::applicationDidFinishLaunching() {
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if (not glview) {
        glview = GLViewImpl::create("Pulse");
        director->setOpenGLView(glview);
    }

    director->setProjection(Director::Projection::_2D);
    director->setDisplayStats(false);
    director->setAnimationInterval(1.0f / 60);

    const auto& designResolution = Resolution::design();
    glview->setDesignResolutionSize(designResolution.size.width, designResolution.size.height, ResolutionPolicy::FIXED_HEIGHT);

    const auto& displayResolution = Resolution::forFrame(glview->getFrameSize());
    director->setContentScaleFactor(displayResolution.contentScaleFactor());
    FileUtils::getInstance()->setSearchPaths(displayResolution.resourceSearchPaths());

    gameRunning(true);
    allocateResources();
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

void AppDelegate::allocateResources() {
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
    titleScene_ = retained<TitleScene>();
    titleScene_->setOnEnterCallback([this]() {
        titleScene_->setBackground(this->sharedAnimatedBackground());
    });
    titleScene_->onPlaySelected = [this](auto scene) {
        this->addGameScene();
        CC_SAFE_RELEASE_NULL(titleScene_);
    };
    titleScene_->onModesSelected = [this](auto scene) {
        const auto modeSelectionScene = autoreleased<ModeSelectionScene>(options_.gameMode);
        modeSelectionScene->onSceneDismissed = [this](auto scene) {
            options_.gameMode = scene->selectedMode();
            scene->removeFromParent();
        };
        scene->addChild(modeSelectionScene, 10);
    };
    titleScene_->onSettingsSelected = [this](auto scene) {
        const auto developerScene = autoreleased<DeveloperMenuScene>(options_);
        developerScene->onSceneDismissed = [](auto scene) {
            Director::getInstance()->popScene();
        };
        Director::getInstance()->pushScene(developerScene);
    };
    Director::getInstance()->replaceScene(titleScene_);
}

void AppDelegate::addGameScene() {
    gameScene_ = retained<GameScene>(options_);
    gameScene_->setOnEnterCallback([this]() {
        gameScene_->setBackground(this->sharedAnimatedBackground());
    });
    gameScene_->onEnterMenu = [this](auto scene) {
        this->addPauseMenuScene();
    };
    gameScene_->onGameOver = [this](auto scene, auto score) {
        this->addGameOverScene(score);
        CC_SAFE_RELEASE_NULL(gameScene_);
    };
    gameScene_->onSceneDismissed = [this](auto scene) {
        this->addTitleScene();
        CC_SAFE_RELEASE_NULL(gameScene_);
    };
    gameScene_->startNewGame();
    Director::getInstance()->replaceScene(gameScene_);
}

void AppDelegate::addPauseMenuScene() {
    const auto menuScene = autoreleased<PauseMenuScene>();
    menuScene->setOnEnterCallback([=]() {
        menuScene->setBackground(this->sharedAnimatedBackground());
    });
    menuScene->onResumeGame = [this](auto scene) {
        Director::getInstance()->popScene();
    };
    menuScene->onRestartGame = [this](auto scene) {
        gameScene_->startNewGame();
        Director::getInstance()->popScene();
    };
    menuScene->onQuitGame = [this](auto scene) {
        Director::getInstance()->popScene();
        gameScene_->onSceneDismissed(gameScene_);
    };
    Director::getInstance()->pushScene(menuScene);
}

void AppDelegate::addGameOverScene(int score) {
    const auto scene = autoreleased<GameOverScene>(score);
    scene->setOnEnterCallback([=]() {
        scene->setBackground(this->sharedAnimatedBackground());
    });
    scene->onRestartGame = [this](auto scene) {
        this->addGameScene();
    };
    scene->onQuitGame = [this](auto scene) {
        this->addTitleScene();
    };
    Director::getInstance()->replaceScene(scene);
}
