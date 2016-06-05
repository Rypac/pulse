#pragma once

#include "cocos2d.h"

namespace pulse {

class FollowedBy : public cocos2d::Action {
  public:
    FollowedBy(cocos2d::Node* follower);
    virtual ~FollowedBy();

    virtual void startWithTarget(cocos2d::Node* target) override;
    virtual void stop() override;
    virtual void step(float dt) override;
    virtual bool isDone() const override;

  private:
    FollowedBy(const FollowedBy&) = delete;
    FollowedBy& operator=(const FollowedBy&) = delete;

    cocos2d::Node* follower_;
};

}
