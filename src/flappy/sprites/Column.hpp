#pragma once

#include "cocos2d.h"

namespace flappy {

class Column : public cocos2d::Sprite {
  public:
    CREATE_FUNC(Column);
    static Column* create(float height);
    static Column* create(cocos2d::Rect frame);

    virtual bool init() override;

    constexpr static const float defaultLength{50.0};
};

}
