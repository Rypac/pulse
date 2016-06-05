#pragma once

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "pulse/scenes/GameScene.hpp"

namespace pulse {

class SplashScene : public GameScene {
  public:
    SplashScene() {}
    virtual ~SplashScene();

    virtual bool init() override;

    using SceneCallback = std::function<void(SplashScene* scene)>;
    SceneCallback onSceneDismissed;

  private:
    cocos2d::Action* logoAnimation();

    cocos2d::Sprite* image;
};

}
