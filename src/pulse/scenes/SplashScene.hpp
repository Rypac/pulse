#pragma once

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "pulse/scenes/GameScene.hpp"

namespace pulse {

class SplashScene : public GameScene {
  public:
    static SplashScene* create();

    using SceneCallback = std::function<void(SplashScene* scene)>;
    SceneCallback onSceneDismissed;

  protected:
    SplashScene() {}
    virtual ~SplashScene();

    virtual bool init() override;

  private:
    cocos2d::Action* logoAnimation();

    cocos2d::Sprite* image;
};

}
