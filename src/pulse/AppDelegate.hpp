#pragma once

#include "pulse/scenes/GameScene.hpp"
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
    void allocateResources();
    void addSplashScene();
    void addTitleScene();
    void addGameScene();
    void addGameOverScene(int score);
    void addPauseMenuScene();

    cocos2d::Node* sharedAnimatedBackground() const;

    pulse::GameOptions options_;
    cocos2d::Node* animatedBackground_;

    pulse::GameScene* gameScene_;
    pulse::TitleScene* titleScene_;
};
