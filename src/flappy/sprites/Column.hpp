#pragma once

#include "cocos2d.h"

namespace flappy {

class Column : public cocos2d::Sprite {
public:
    CREATE_FUNC(Column);
    static Column* create(float height);
    static Column* create(cocos2d::Rect frame);

    static const int gapHeight = 100;
    static const int defaultWidth = 50;

    virtual bool init() override;

    cocos2d::Sequence* actionSequence(float duration = 5);
};

}
