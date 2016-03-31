#pragma once

#include "cocos2d.h"
#include "flappy/scenes/GameScene.hpp"

namespace flappy {

class DeveloperMenuScene : public GameScene {
  public:
    CREATE_FUNC(DeveloperMenuScene);

    bool init() override;

    std::function<void(DeveloperMenuScene* scene)> onSceneDismissed;
};

}
