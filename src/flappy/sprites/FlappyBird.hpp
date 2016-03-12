#pragma once

#include "cocos2d.h"

namespace flappy {

class FlappyBird : public cocos2d::Sprite {
public:
    CREATE_FUNC(FlappyBird);

    virtual bool init() override;

    cocos2d::Vec2 velocity{};
};
    
}
