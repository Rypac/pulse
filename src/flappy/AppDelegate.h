#pragma once

#include "cocos2d.h"
#include "flappy/models/GameOptions.hpp"

class AppDelegate : private cocos2d::Application {
  public:
    AppDelegate();
    virtual ~AppDelegate();

    virtual void initGLContextAttrs();
    virtual bool applicationDidFinishLaunching();
    virtual void applicationDidEnterBackground();
    virtual void applicationWillEnterForeground();

  private:
    flappy::GameOptions options;
};
