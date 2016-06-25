#pragma once

#include "cocos2d.h"
#include <type_traits>

template <typename T, typename... Args, typename = typename std::enable_if<std::is_base_of<cocos2d::Node, T>::value>::type>
inline T* create(Args&&... args) {
    auto object = new (std::nothrow) T(std::forward<Args>(args)...);
    if (object and object->init()) {
        object->autorelease();
        return object;
    }
    delete object;
    return nullptr;
}

namespace node {

void stopAllActionsRecursively(cocos2d::Node* node) {
    for (auto&& child : node->getChildren()) {
        stopAllActionsRecursively(child);
    }
    node->stopAllActions();
}

}
