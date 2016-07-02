#pragma once

#include "pulse/scenes/BaseScene.hpp"
#include "pulse/utilities/NodeAnimator.hpp"

namespace pulse {

class GameOverScene : public BaseScene {
  public:
    GameOverScene(): GameOverScene(0) {}
    GameOverScene(int score);

    using SceneCallback = std::function<void(GameOverScene* scene)>;
    SceneCallback onRestartGame;
    SceneCallback onQuitGame;

  private:
    void addBanner(int score);
    void addRestartButton();
    void addHomeButton();

    cocos2d::FiniteTimeAction* entryAnimation();
    cocos2d::FiniteTimeAction* exitAnimation();

    NodeAnimator scoreAnimator_;
    NodeAnimator restartAnimator_;
    NodeAnimator homeAnimator_;
};

}
