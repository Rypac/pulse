#pragma once

#include "cocos2d.h"
#include <gsl/gsl.h>

namespace flappy {

class GameScene : public cocos2d::Layer {
public:
    CREATE_FUNC(GameScene);

    static gsl::owner<cocos2d::Scene*> createScene();
    virtual bool init() override;
    virtual void reset() {};

    inline cocos2d::Rect getFrame() { return frame; };

    bool isScenePaused();
    void pauseScene();
    void resumeScene();

protected:
    cocos2d::Rect frame;
};

}
