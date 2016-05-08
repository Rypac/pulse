#pragma once

#include "cocos2d.h"

namespace pulse {

class FollowedBy : public cocos2d::Action {
  public:
    static FollowedBy* create(cocos2d::Node* follower);

    virtual void startWithTarget(cocos2d::Node* target) override;
    virtual void stop() override;
    virtual void step(float dt) override;
    virtual bool isDone() const override;

  protected:
    FollowedBy(cocos2d::Node* follower): follower{follower} {}
    virtual ~FollowedBy();

    cocos2d::Node* follower;

  private:
    FollowedBy(const FollowedBy&) = delete;
    FollowedBy& operator=(const FollowedBy&) = delete;
};

}
