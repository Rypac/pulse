#pragma once

#include "cocos2d.h"
#include "pulse/models/GameOptions.hpp"
#include "pulse/scenes/PulseGameScene.hpp"
#include "pulse/scenes/TitleScene.hpp"
#include "pulse/controllers/SplashViewController.hpp"

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
    void addTitleScene();
    void addGameScene();
    void addInGameMenuScene();

    pulse::GameOptions options;

    pulse::PulseGameScene* gameScene;
    pulse::TitleScene* titleScene;
    pulse::SplashViewController splashController_;
};
