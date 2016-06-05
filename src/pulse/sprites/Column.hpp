#pragma once

#include "cocos2d.h"

namespace pulse {

class Column : public cocos2d::Sprite {
  public:
    Column();
    Column(float height);
    Column(const cocos2d::Rect& frame);

    constexpr static const float defaultLength{50.0};
};

}
