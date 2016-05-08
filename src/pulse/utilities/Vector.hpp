#pragma once

#include "cocos2d.h"

namespace pulse {

template <typename T>
static inline cocos2d::Vector<T> toVector(const std::vector<T>& vec) {
    cocos2d::Vector<T> out;
    for (const auto& element : vec) {
        out.pushBack(element);
    }
    return out;
}

}