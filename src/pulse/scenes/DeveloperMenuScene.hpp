#pragma once

#include <vector>
#include "pulse/models/GameOptions.hpp"
#include "pulse/scenes/BaseScene.hpp"
#include "pulse/ui/MenuSlider.hpp"

namespace pulse {

class DeveloperMenuScene : public BaseScene {
  public:
    DeveloperMenuScene(GameOptions& options);

    using SceneCallback = std::function<void(DeveloperMenuScene* scene)>;
    SceneCallback onSceneDismissed;

  private:
    void addSliders(std::vector<ui::MenuSlider*> sliders);
    void addBackButton();
};

}
