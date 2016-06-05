#pragma once

#include "cocos2d.h"

namespace pulse {

class RemoveSelfWithCallback : public cocos2d::ActionInstant {
  public:
    using Callback = std::function<void(void)>;
    RemoveSelfWithCallback(Callback callback = nullptr): onRemoved(callback) {}

    Callback onRemoved;

    virtual void update(float time) override;
    virtual RemoveSelfWithCallback* clone() const override;
    virtual RemoveSelfWithCallback* reverse() const override;

  private:
    RemoveSelfWithCallback(const RemoveSelfWithCallback&) = delete;
    RemoveSelfWithCallback& operator=(const RemoveSelfWithCallback&) = delete;
};

}
