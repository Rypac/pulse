#pragma once

#include "cocos2d.h"
#include <type_traits>

template <typename T, typename... Args, typename = typename std::enable_if<std::is_base_of<cocos2d::Ref, T>::value>::type>
inline T* owned(Args&&... args) {
    return new (std::nothrow) T(std::forward<Args>(args)...);
}

template <typename T, typename... Args, typename = typename std::enable_if<std::is_base_of<cocos2d::Ref, T>::value>::type>
inline T* autoreleased(Args&&... args) {
    auto object = owned<T>(std::forward<Args>(args)...);
    if (object) {
        object->autorelease();
    }
    return object;
}

template <typename T, typename... Args, typename = typename std::enable_if<std::is_base_of<cocos2d::Ref, T>::value>::type>
inline T* retained(Args&&... args) {
    auto object = autoreleased<T>(std::forward<Args>(args)...);
    if (object) {
        object->retain();
    }
    return object;
}
