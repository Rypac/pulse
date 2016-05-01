#pragma once

#include "cocos2d.h"
#include "pulse/models/GameOptions.hpp"

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
    void addSplashScene();
    void addTitleScene();
    void addGameScene();
    void addDeveloperMenuScene();

    pulse::GameOptions options;
};
