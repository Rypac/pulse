#pragma once

#include "cocos2d.h"
#include "pulse/scenes/GameScene.hpp"

namespace pulse {

class TitleScene : public GameScene {
public:
    static TitleScene* create();

    virtual bool init() override;
    virtual void onEnter() override;

    using SceneCallback = std::function<void(TitleScene* scene)>;
    SceneCallback onSceneDismissed;

private:
    cocos2d::Action* titleScreenAnimation();

    cocos2d::Sprite3D* title;
};

}
