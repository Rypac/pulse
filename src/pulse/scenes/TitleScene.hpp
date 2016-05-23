#pragma once

#include "cocos2d.h"
#include "pulse/scenes/GameScene.hpp"
#include "pulse/ui/Button.hpp"

namespace pulse {

class TitleScene : public GameScene {
  public:
    static TitleScene* create();

    virtual void update(float dt) override;

    using SceneCallback = std::function<void(TitleScene* scene)>;
    SceneCallback onSceneDismissed;

  protected:
    TitleScene();
    virtual ~TitleScene();
    virtual bool init() override;

  private:
    void addTitle();
    void addTitleAnimation();
    void addPlayButton();
    void addModesButton();
    void addAchievmentsButton();
    void addSettingsButton();

    cocos2d::Sprite3D* title;
    cocos2d::Sequence* titleAnimation;
    float animationStep;
};

}
