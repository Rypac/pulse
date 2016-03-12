#pragma once

#include "cocos2d.h"
#include <gsl/gsl.h>

class GameScene : public cocos2d::Layer {
public:
    CREATE_FUNC(GameScene);

    static gsl::owner<cocos2d::Scene*> createScene();
    virtual bool init() override;

    inline cocos2d::Rect getFrame() { return frame; };

protected:
    cocos2d::Rect frame;
};
