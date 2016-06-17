#pragma once

#include "pulse/scenes/BaseScene.hpp"

namespace pulse {

class SplashScene : public BaseScene {
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
