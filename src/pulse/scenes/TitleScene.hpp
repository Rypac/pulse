#pragma once

#include "cocos2d.h"
#include "pulse/scenes/GameScene.hpp"

namespace pulse {

class TitleScene : public GameScene {
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
