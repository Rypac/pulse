#pragma once

#include "pulse/scenes/GameScene.hpp"

namespace pulse {

class InGameMenuScene : public GameScene {
  public:
    static InGameMenuScene* create();

    using SceneCallback = std::function<void(InGameMenuScene* scene)>;
    SceneCallback onResumeGame;
    SceneCallback onRestartGame;
    SceneCallback onQuitGame;

  protected:
    InGameMenuScene() {}
    virtual ~InGameMenuScene() {}

    virtual bool init() override;

  private:
    void addResumeButton();
    void addRestartButton();
    void addQuitButton();
};

}
