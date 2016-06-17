#pragma once

#include "pulse/scenes/BaseScene.hpp"

namespace pulse {

class TitleScene : public BaseScene {
  public:
    TitleScene();

    using SceneCallback = std::function<void(TitleScene* scene)>;
    SceneCallback onPlaySelected;
    SceneCallback onModesSelected;
    SceneCallback onAchievementsSelected;
    SceneCallback onSettingsSelected;

  private:
    void addTitle();
    void addPlayButton();
    void addModesButton();
    void addAchievmentsButton();
    void addSettingsButton();
};

}
