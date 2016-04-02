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

    using SceneDismissedCallback = std::function<void(SplashScene* scene)>;
    SceneDismissedCallback onSceneDismissed;

  private:
    cocos2d::experimental::ui::VideoPlayer* videoPlayer;
};

}
