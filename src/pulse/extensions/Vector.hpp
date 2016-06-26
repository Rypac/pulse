#pragma once

#include "cocos2d.h"

namespace pulse {

template <typename T>
inline cocos2d::Vector<T> toVector(const std::vector<T>& vec) {
    cocos2d::Vector<T> out;
    for (auto&& element : vec) {
        out.pushBack(element);
    }
    return out;
}

}
