#pragma once

#include "cocos2d.h"
#include "pulse/models/GameOptions.hpp"

class AppDelegate : private cocos2d::Application {
  public:
    AppDelegate();
    virtual ~AppDelegate();

    virtual void initGLContextAttrs();
    virtual bool applicationDidFinishLaunching();
    virtual void applicationDidEnterBackground();
    virtual void applicationWillEnterForeground();

  private:
    void addSplashScene();
    void addGameScene();
    void addDeveloperMenuScene();

    pulse::GameOptions options;
};
