#pragma once

#include "cocos2d.h"

namespace pulse {

template <typename T>
class Generator {
  public:
    virtual T generate() const = 0;
};

}