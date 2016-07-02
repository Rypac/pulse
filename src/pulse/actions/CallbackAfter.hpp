#pragma once

#include "cocos2d.h"

namespace pulse {

class CallbackAfter : public cocos2d::Sequence {
  public:
    CallbackAfter(cocos2d::FiniteTimeAction* action, const std::function<void()>& callback) {
        initWithTwoActions(action, cocos2d::CallFunc::create(callback));
    }

    CallbackAfter(cocos2d::FiniteTimeAction* action, const std::function<void(cocos2d::Node*)>& callback) {
        initWithTwoActions(action, cocos2d::CallFuncN::create(callback));
    }
};

class CallbackAfterDelay : public cocos2d::Sequence {
  public:
    CallbackAfterDelay(float delay, const std::function<void()>& callback) {
        initWithTwoActions(cocos2d::DelayTime::create(delay), cocos2d::CallFunc::create(callback));
    }
};

}
