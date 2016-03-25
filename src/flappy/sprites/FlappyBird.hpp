#pragma once

#include "cocos2d.h"

namespace flappy {

class FlappyBird : public cocos2d::Sprite {
public:
    CREATE_FUNC(FlappyBird);

    virtual bool init() override;
    virtual void update(float dt) override;

    cocos2d::Vec2 velocity{0, 0};
};

}
