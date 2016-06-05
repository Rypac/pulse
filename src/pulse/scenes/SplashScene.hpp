#pragma once

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "pulse/scenes/GameScene.hpp"

namespace pulse {

class SplashScene : public GameScene {
  public:
    SplashScene();
    virtual ~SplashScene();

    virtual void onEnterTransitionDidFinish() override;

    using SceneCallback = std::function<void(SplashScene* scene)>;
    SceneCallback onSceneDismissed;

  private:
    cocos2d::Sprite* image_;
};

}
