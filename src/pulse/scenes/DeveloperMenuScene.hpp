#pragma once

#include <vector>
#include "cocos2d.h"
#include "pulse/models/GameOptions.hpp"
#include "pulse/scenes/GameScene.hpp"
#include "pulse/ui/MenuSlider.hpp"

namespace pulse {

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