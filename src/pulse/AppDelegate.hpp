#pragma once

#include "cocos2d.h"
#include "pulse/models/GameOptions.hpp"
#include "pulse/scenes/PulseGameScene.hpp"
#include "pulse/scenes/TitleScene.hpp"

class AppDelegate : private cocos2d::Application {
  public:
    AppDelegate();
    virtual ~AppDelegate();

    virtual void initGLContextAttrs() override;
    virtual bool applicationDidFinishLaunching() override;
    virtual void applicationDidEnterBackground() override;
    virtual void applicationWillEnterForeground() override;

  private:
    void gameRunning(bool running);
    void allocatedResources();
    void addSplashScene();
    void addTitleScene();
    void addGameScene();
    void addInGameMenuScene();

    cocos2d::Node* sharedAnimatedBackground() const;

    pulse::GameOptions options;
    cocos2d::Node* animatedBackground_;

    pulse::PulseGameScene* gameScene;
    pulse::TitleScene* titleScene;
};
