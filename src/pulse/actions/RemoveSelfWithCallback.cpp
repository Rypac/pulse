#include "pulse/actions/RemoveSelfWithCallback.hpp"
#include "pulse/extensions/Ref.hpp"
#include "pulse/utilities/Callback.hpp"

using namespace pulse;
using namespace cocos2d;

void RemoveSelfWithCallback::update(float time) {
    _target->removeFromParentAndCleanup(true);
    safe_callback(onRemoved);
}

RemoveSelfWithCallback* RemoveSelfWithCallback::reverse() const {
    return autoreleased<RemoveSelfWithCallback>(onRemoved);
}

RemoveSelfWithCallback* RemoveSelfWithCallback::clone() const {
    return autoreleased<RemoveSelfWithCallback>(onRemoved);
}
