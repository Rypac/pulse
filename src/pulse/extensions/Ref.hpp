#pragma once

#include "cocos2d.h"
#include <type_traits>

template <typename T, typename... Args, typename = typename std::enable_if<std::is_base_of<cocos2d::Ref, T>::value>::type>
inline T* autoreleased(Args&&... args) {
    auto object = new (std::nothrow) T(std::forward<Args>(args)...);
    object->autorelease();
    return object;
}
