#pragma once

#include "cocos2d.h"

namespace flappy {

class GameScene : public cocos2d::Layer {
  public:
    enum class Status {
        Initialising,
        Running,
        Paused,
        Stopped
    };

    static cocos2d::Scene* createScene();
    static cocos2d::Scene* createScene(GameScene* scene);
    static cocos2d::Scene* createPhysicsScene(GameScene* scene, cocos2d::Vec2 gravity = cocos2d::Vec2{}, bool autoStep = false);

    virtual bool init() override;
    virtual void initScene();
    virtual void clearScene();
    virtual void resetScene();

    void pauseScene();
    void resumeScene();
    void stopScene();
    void exitScene();

    bool residesInSceneBounds(const cocos2d::Node& node) const;

    Status sceneStatus() const {
        return status;
    }

    cocos2d::Rect getFrame() const {
        return frame;
    };

  protected:
    cocos2d::Rect frame;

  private:
    Status status;
};

}
