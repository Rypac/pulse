#pragma once

#include <vector>
#include "cocos2d.h"
#include "pulse/models/GameOptions.hpp"
#include "pulse/scenes/GameScene.hpp"
#include "pulse/ui/MenuSlider.hpp"

namespace pulse {

class DeveloperMenuScene : public GameScene {
  public:
    DeveloperMenuScene(GameOptions& options);

    using SceneCallback = std::function<void(DeveloperMenuScene* scene)>;
    SceneCallback onSceneDismissed;

  private:
    void addSliders(std::vector<ui::MenuSlider*> sliders);
    void addExitButton();

    GameOptions& options;
};

}
