#pragma once

#include "cocos2d.h"

namespace pulse {

class GameScene : public cocos2d::Layer {
  public:
    enum class Status {
        Running,
        Stopped
    };

    static cocos2d::Scene* createScene();
    static cocos2d::Scene* createScene(GameScene* scene);
    static cocos2d::Scene* createPhysicsScene(GameScene* scene, cocos2d::Vec2 gravity = cocos2d::Vec2{}, bool autoStep = false);

    virtual bool init() override;
    virtual void onEnter() override;
    virtual void onExit() override;
    virtual void runScene();
    virtual void stopScene();
    virtual void quit();

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
