#pragma once

#include "cocos2d.h"

namespace pulse {

class AnimatedBackground : public cocos2d::Action {
  public:
    AnimatedBackground(cocos2d::ParticleSystem* particles);
    virtual ~AnimatedBackground();

    AnimatedBackground(const AnimatedBackground&) = delete;
    AnimatedBackground& operator=(const AnimatedBackground&) = delete;

    virtual void startWithTarget(cocos2d::Node* target) override;
    virtual void stop() override;
    virtual void step(float dt) override {}
    virtual bool isDone() const override;

  private:
    cocos2d::ParticleSystem* particles_;
};

}  // pulse
