#pragma once

#include "pulse/scenes/GameScene.hpp"

namespace pulse {

class PauseMenuScene : public GameScene {
  public:
    PauseMenuScene();

    using SceneCallback = std::function<void(PauseMenuScene* scene)>;
    SceneCallback onResumeGame;
    SceneCallback onRestartGame;
    SceneCallback onQuitGame;

  private:
    void addResumeButton();
    void addRestartButton();
    void addQuitButton();
};

}
