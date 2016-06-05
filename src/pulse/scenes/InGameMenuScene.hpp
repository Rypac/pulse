#pragma once

#include "pulse/scenes/GameScene.hpp"

namespace pulse {

class InGameMenuScene : public GameScene {
  public:
    InGameMenuScene() {}
    virtual ~InGameMenuScene() {}

    virtual bool init() override;

    using SceneCallback = std::function<void(InGameMenuScene* scene)>;
    SceneCallback onResumeGame;
    SceneCallback onRestartGame;
    SceneCallback onQuitGame;

  private:
    void addResumeButton();
    void addRestartButton();
    void addQuitButton();
};

}
