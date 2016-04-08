#pragma once

#include "cocos2d.h"

namespace pulse {

class Pulse : public cocos2d::Sprite {
public:
    CREATE_FUNC(Pulse);
    static Pulse* create(cocos2d::Size size);
    static Pulse* create(cocos2d::Rect frame);

    virtual bool init() override;
};

}
