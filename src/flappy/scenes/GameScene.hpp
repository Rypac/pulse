#pragma once

#include "cocos2d.h"

class GameScene : public cocos2d::Layer {
public:
    inline cocos2d::Rect getFrame() { return frame; };

protected:
    cocos2d::Rect frame;
};
