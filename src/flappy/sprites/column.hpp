#pragma once

#include "cocos2d.h"

namespace flappy {

class Column : public cocos2d::Sprite {
public:
    CREATE_FUNC(Column);
    static Column* create(cocos2d::Rect sceneFrame);

    virtual bool init() override;

    cocos2d::Sequence* actionSequence();

};

}
