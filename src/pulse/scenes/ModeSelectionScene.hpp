#pragma once

#include "pulse/scenes/GameScene.hpp"
#include "pulse/models/GameMode.hpp"
#include "pulse/ui/Button.hpp"

namespace pulse {

class ModeSelectionScene : public GameScene {
  public:
    static ModeSelectionScene* create(GameMode mode = GameMode::FreePlay);

    using SceneCallback = std::function<void(ModeSelectionScene* scene)>;
    SceneCallback onSceneDismissed;

    GameMode selectedMode() const {
        return mode_;
    }

  protected:
    ModeSelectionScene(GameMode mode): mode_{mode} {}
    virtual ~ModeSelectionScene() {}

    virtual bool init() override;

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

}
