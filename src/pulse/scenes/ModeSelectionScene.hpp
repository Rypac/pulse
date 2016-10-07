#pragma once

#include "pulse/models/GameMode.hpp"
#include "pulse/scenes/BaseScene.hpp"
#include "pulse/ui/Button.hpp"

namespace pulse {

class ModeSelectionScene : public BaseScene {
  public:
    ModeSelectionScene(GameMode mode = GameMode::FreePlay);

    virtual void onEnterTransitionDidFinish() override;

    using SceneCallback = std::function<void(ModeSelectionScene* scene)>;
    SceneCallback onSceneDismissed;

    GameMode selectedMode() const {
        return mode_;
    }

  private:
    struct Mode {
        Mode(GameMode mode, const std::string& name);

        GameMode mode;
        cocos2d::Label* label;
        pulse::ui::Button* button;
    };

    void updateSelectedMode(GameMode mode);
    void addDismissListener();
    void layoutModes();

    std::vector<ModeSelectionScene::Mode> modes_;
    GameMode mode_;
};

}  // pulse
