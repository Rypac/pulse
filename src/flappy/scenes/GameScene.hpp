#pragma once

#include "cocos2d.h"
#include <gsl/gsl.h>

namespace flappy {

class GameScene : public cocos2d::Layer {
  public:
    enum class Status {
        Running, Paused, Stopped
    };

    CREATE_FUNC(GameScene);

    static gsl::owner<cocos2d::Scene*> createScene();
    virtual bool init() override;

    virtual void initScene();
    virtual void clearScene();
    virtual void resetScene();

    void pauseScene();
    void resumeScene();
    void stopScene();
    void exitScene();

    inline Status sceneStatus() { return status; }
    inline cocos2d::Rect getFrame() { return frame; };

  protected:
    cocos2d::Rect frame;

  private:
    Status status;
};

}
