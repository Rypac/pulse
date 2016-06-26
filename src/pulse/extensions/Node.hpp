#pragma once

#include "cocos2d.h"
#include <type_traits>

template <typename T, typename... Args, typename = std::enable_if_t<std::is_base_of<cocos2d::Node, T>::value>>
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

inline cocos2d::Rect frame(cocos2d::Node* node) {
    return cocos2d::Rect{cocos2d::Vec2::ZERO, node->getContentSize()};
}

inline bool containsPoint(cocos2d::Node* node, const cocos2d::Vec2& point) {
    const auto nodeSpacePoint = PointApplyTransform(point, node->getWorldToNodeTransform());
    return frame(node).containsPoint(nodeSpacePoint);
}

inline void stopAllActionsRecursively(cocos2d::Node* node) {
    for (auto&& child : node->getChildren()) {
        stopAllActionsRecursively(child);
    }
    node->stopAllActions();
}

}
