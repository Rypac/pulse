#pragma once

#include "cocos2d.h"
#include "pulse/scenes/GameScene.hpp"

namespace pulse {

class TitleScene : public GameScene {
  public:
    static TitleScene* create();

    virtual void onEnter() override;
    virtual void update(float dt) override;

    using SceneCallback = std::function<void(TitleScene* scene)>;
    SceneCallback onSceneDismissed;

  protected:
    virtual bool init() override;

  private:
    void addBackground();
    void addTitle();
    void addPlayButton();
    void addTouchListeners();
    cocos2d::Sequence* createTitleAnimation();

    cocos2d::Sprite3D* title;
    cocos2d::Sequence* titleAnimation;
    float animationStep;

    cocos2d::Sprite* playButton;
};

}
