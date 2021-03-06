#pragma once

#include "cocos2d.h"

namespace pulse {

class FollowedBy : public cocos2d::Action {
  public:
    FollowedBy(cocos2d::Node* follower, const cocos2d::Vec2& offset = cocos2d::Vec2::ZERO);
    virtual ~FollowedBy();

    FollowedBy(const FollowedBy&) = delete;
    FollowedBy& operator=(const FollowedBy&) = delete;

    virtual void startWithTarget(cocos2d::Node* target) override;
    virtual void stop() override;
    virtual void step(float dt) override;
    virtual bool isDone() const override;

  private:
    cocos2d::Node* follower_;
    cocos2d::Vec2 offset_;
};

}  // pulse
