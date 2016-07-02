#pragma once

#include "pulse/scenes/BaseScene.hpp"
#include "pulse/utilities/NodeAnimator.hpp"

namespace pulse {

class GameOverScene : public BaseScene {
  public:
    GameOverScene();

    using SceneCallback = std::function<void(GameOverScene* scene)>;
    SceneCallback onRestartGame;

  private:
    void addBanner();
    void addRestartButton();

    cocos2d::Action* entryAnimation();
    cocos2d::Action* exitAnimation();

    NodeAnimator scoreAnimator_;
    NodeAnimator restartAnimator_;
};

}
