#pragma once

#include "cocos2d.h"

namespace flappy {

template <typename T>
class Generator {
  public:
    virtual T generate() const = 0;
};

}
