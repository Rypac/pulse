#pragma once

#include "cocos2d.h"

namespace pulse {

class RemoveSelfWithCallback : public cocos2d::ActionInstant {
  public:
    using Callback = std::function<void(void)>;
    static RemoveSelfWithCallback* create(Callback callback = nullptr);

    Callback onRemoved;

    virtual void update(float time) override;
    virtual RemoveSelfWithCallback* clone() const override;
    virtual RemoveSelfWithCallback* reverse() const override;

  protected:
    RemoveSelfWithCallback(): onRemoved(nullptr) {}
    RemoveSelfWithCallback(Callback callback): onRemoved(callback) {}
    virtual ~RemoveSelfWithCallback() {}

  private:
    RemoveSelfWithCallback(const RemoveSelfWithCallback&) = delete;
    RemoveSelfWithCallback& operator=(const RemoveSelfWithCallback&) = delete;
};

}
