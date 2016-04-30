#pragma once

#include "cocos2d.h"
#include "pulse/scenes/GameScene.hpp"

namespace pulse {

class TitleScene : public GameScene {
public:
    static TitleScene* create();

    virtual bool init() override;
    virtual void onEnter() override;
    virtual void update(float dt) override;
    virtual bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event) override;
    virtual void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event) override;

    using SceneCallback = std::function<void(TitleScene* scene)>;
    SceneCallback onSceneDismissed;

private:
    void addTouchListeners();
    cocos2d::Sequence* titleScreenAnimation();

    cocos2d::Sprite3D* title;
    cocos2d::Sequence* titleAnimation;
    float animationStep;
};

}
