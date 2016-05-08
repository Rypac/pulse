#include "pulse/actions/RemoveSelfWithCallback.hpp"

using namespace pulse;
using namespace cocos2d;

RemoveSelfWithCallback * RemoveSelfWithCallback::create(Callback callback) {
    RemoveSelfWithCallback *action = new (std::nothrow) RemoveSelfWithCallback();
    if (action && action->init(callback)) {
        action->autorelease();
        return action;
    }
    delete action;
    return nullptr;
}

bool RemoveSelfWithCallback::init(Callback callback) {
    if (!RemoveSelf::init(true)) {
        return false;
    }
    onRemoved = callback;
    return true;
}

void RemoveSelfWithCallback::update(float time) {
    RemoveSelf::update(time);
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
