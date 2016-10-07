#pragma once

#include "pulse/scenes/BaseScene.hpp"

namespace pulse {

class SplashScene : public BaseScene {
  public:
    SplashScene();
    virtual ~SplashScene();

    using SceneCallback = std::function<void(SplashScene* scene)>;
    SceneCallback onSceneDismissed;

  private:
    void playLogoAnimation();

    cocos2d::Sprite* image_;
};

}  // pulse
