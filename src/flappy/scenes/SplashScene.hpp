#pragma once

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "flappy/scenes/GameScene.hpp"

namespace flappy {

class SplashScene : public GameScene {
  public:
    static SplashScene* create();

    virtual bool init() override;
    virtual void onEnter() override;

    using SceneCallback = std::function<void(SplashScene* scene)>;
    SceneCallback onSceneDismissed;

  private:
    cocos2d::experimental::ui::VideoPlayer* videoPlayer;
};

}
