#pragma once

#include "pulse/scenes/GameScene.hpp"

namespace pulse {

class GameOverScene : public GameScene {
  public:
    GameOverScene();

    using SceneCallback = std::function<void(GameOverScene* scene)>;
    SceneCallback onRestartGame;

  private:
    void addBanner();
    void addRestartButton();
};

}
