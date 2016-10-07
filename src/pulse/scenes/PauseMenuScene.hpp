#pragma once

#include "pulse/scenes/BaseScene.hpp"

namespace pulse {

class PauseMenuScene : public BaseScene {
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

}  // pulse
