#pragma once

#include <type_traits>
#include "cocos2d.h"

namespace pulse {

template <typename T, typename... Args, typename = std::enable_if_t<std::is_base_of<cocos2d::Ref, T>::value>>
inline T* owned(Args&&... args) {
    return new (std::nothrow) T(std::forward<Args>(args)...);
}

template <typename T, typename... Args, typename = std::enable_if_t<std::is_base_of<cocos2d::Ref, T>::value>>
inline T* autoreleased(Args&&... args) {
    auto object = owned<T>(std::forward<Args>(args)...);
    if (object) {
        object->autorelease();
    }
    return object;
}

template <typename T, typename... Args, typename = std::enable_if_t<std::is_base_of<cocos2d::Ref, T>::value>>
inline T* retained(Args&&... args) {
    auto object = autoreleased<T>(std::forward<Args>(args)...);
    if (object) {
        object->retain();
    }
    return object;
}

}  // pulse
