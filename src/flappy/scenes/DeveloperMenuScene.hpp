#pragma once

#include <vector>
#include "cocos2d.h"
#include "flappy/models/GameOptions.hpp"
#include "flappy/scenes/GameScene.hpp"
#include "flappy/ui/MenuSlider.hpp"

namespace flappy {

class DeveloperMenuScene : public GameScene {
  public:
    using SceneCallback = std::function<void(DeveloperMenuScene* scene)>;
    static cocos2d::Scene* createScene(GameOptions& options, SceneCallback onSceneDismissed = nullptr);
    static DeveloperMenuScene* create(GameOptions& options);

    bool init(GameOptions& options);

    SceneCallback onSceneDismissed;

  private:
    void addSliders();

    std::vector<ui::MenuSlider*> sliders;
};

}
