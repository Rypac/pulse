#pragma once

#include "cocos2d.h"

namespace pulse {

class CallbackAfter : public cocos2d::Sequence {
  public:
    CallbackAfter(cocos2d::FiniteTimeAction* action, std::function<void()> callback) {
        initWithTwoActions(action, cocos2d::CallFunc::create(callback));
    }

    CallbackAfter(cocos2d::FiniteTimeAction* action, std::function<void(cocos2d::Node*)> callback) {
        initWithTwoActions(action, cocos2d::CallFuncN::create(callback));
    }
};

}
