#pragma once

#include "cocos2d.h"

namespace pulse {

class AnimatedBackground : public cocos2d::Action {
  public:
    static AnimatedBackground* create(cocos2d::ParticleSystem* particles);

    virtual void startWithTarget(cocos2d::Node* target) override;
    virtual void stop() override;
    virtual void step(float dt) override {}
    virtual bool isDone() const override;

  protected:
    AnimatedBackground(cocos2d::ParticleSystem* particles): particles{particles} {}
    virtual ~AnimatedBackground();

    cocos2d::ParticleSystem* particles;

  private:
    AnimatedBackground(const AnimatedBackground&) = delete;
    AnimatedBackground& operator=(const AnimatedBackground&) = delete;
};
    
}
