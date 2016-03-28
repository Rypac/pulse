#pragma once

#include "cocos2d.h"

namespace flappy {

class FlappyBird : public cocos2d::Sprite {
public:
    CREATE_FUNC(FlappyBird);
    static FlappyBird* create(cocos2d::Size size);
    static FlappyBird* create(cocos2d::Rect frame);

    virtual bool init() override;
};

}
