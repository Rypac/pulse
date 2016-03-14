#pragma once

#include "cocos2d.h"
#include <gsl/gsl.h>

namespace flappy {

class GameScene : public cocos2d::Layer {
  public:
    CREATE_FUNC(GameScene);

    static gsl::owner<cocos2d::Scene*> createScene();
    virtual bool init() override;

    virtual void initScene() {};
    virtual void clearScene() {};
    virtual void resetScene();

    bool isScenePaused();
    void pauseScene();
    void resumeScene();
    void exitScene();

    inline cocos2d::Rect getFrame() { return frame; };

  protected:
    cocos2d::Rect frame;
};

}
