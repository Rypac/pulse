#include "pulse/actions/RemoveSelfWithCallback.hpp"

using namespace pulse;
using namespace cocos2d;

RemoveSelfWithCallback* RemoveSelfWithCallback::create(Callback callback) {
    const auto action = new (std::nothrow) RemoveSelfWithCallback(callback);
    if (action) {
        action->autorelease();
    }
    return action;
}

void RemoveSelfWithCallback::update(float time) {
    _target->removeFromParentAndCleanup(true);
    if (onRemoved) {
        onRemoved();
    }
}

RemoveSelfWithCallback* RemoveSelfWithCallback::reverse() const {
    return RemoveSelfWithCallback::create(onRemoved);
}

RemoveSelfWithCallback* RemoveSelfWithCallback::clone() const {
    return RemoveSelfWithCallback::create(onRemoved);
}
