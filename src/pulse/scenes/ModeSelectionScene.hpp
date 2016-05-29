#pragma once

#include "pulse/scenes/GameScene.hpp"
#include "pulse/ui/Button.hpp"

namespace pulse {

class ModeSelectionScene : public GameScene {
  public:
    static ModeSelectionScene* create();

    using SceneCallback = std::function<void(ModeSelectionScene* scene)>;
    SceneCallback onSceneDismissed;

  protected:
    ModeSelectionScene() {}
    virtual ~ModeSelectionScene() {}

    virtual bool init() override;

  private:
    ui::Button* resumeButton();
    ui::Button* restartButton();
    ui::Button* quitButton();
    void updateSelectedMode(ui::Button* mode);
    void addDismissListener();

    std::vector<ui::Button*> modeButtons;
};

}
