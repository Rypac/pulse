#pragma once

#include <vector>
#include "cocos2d.h"
#include "flappy/models/GameOptions.hpp"
#include "flappy/scenes/GameScene.hpp"
#include "flappy/ui/MenuSlider.hpp"

namespace flappy {

class DeveloperMenuScene : public GameScene {
  public:
    DeveloperMenuScene(GameOptions& options): options(options) {}

    static DeveloperMenuScene* create(GameOptions& options);

    virtual bool init() override;

    using SceneCallback = std::function<void(DeveloperMenuScene* scene)>;
    SceneCallback onSceneDismissed;

  private:
    void addSliders();
    void addExitButton();

    GameOptions& options;
    std::vector<ui::MenuSlider*> sliders;
};

}
