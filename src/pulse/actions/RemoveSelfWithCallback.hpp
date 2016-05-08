#pragma once

#include "cocos2d.h"

namespace pulse {

class RemoveSelfWithCallback : public cocos2d::RemoveSelf {
  public:
    using Callback = std::function<void(void)>;
    static RemoveSelfWithCallback* create(Callback callback = nullptr);

    virtual void update(float time) override;
    virtual RemoveSelfWithCallback* clone() const override;
    virtual RemoveSelfWithCallback* reverse() const override;

  protected:
    RemoveSelfWithCallback() : onRemoved(nullptr) {}
    virtual ~RemoveSelfWithCallback() {}

    bool init(Callback callback);

    Callback onRemoved;

  private:
    RemoveSelfWithCallback(const RemoveSelfWithCallback&) = delete;
    RemoveSelfWithCallback& operator=(const RemoveSelfWithCallback&) = delete;
};

}
