#pragma once

#include "pulse/scenes/BaseScene.hpp"

namespace pulse {

class GameOverScene : public BaseScene {
  public:
    GameOverScene();

    using SceneCallback = std::function<void(GameOverScene* scene)>;
    SceneCallback onRestartGame;

  private:
    void addBanner();
    void addRestartButton();
};

}
