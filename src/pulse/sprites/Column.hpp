#pragma once

#include "cocos2d.h"

namespace pulse {

class Column : public cocos2d::Sprite {
  public:
    static Column* create();
    static Column* create(float height);
    static Column* create(cocos2d::Rect frame);

    virtual bool init() override;

    constexpr static const float defaultLength{50.0};
};

}
