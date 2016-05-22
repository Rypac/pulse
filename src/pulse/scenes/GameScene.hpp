#pragma once

#include "cocos2d.h"

namespace pulse {

class GameScene : public cocos2d::Layer {
  public:
    static cocos2d::Scene* createScene(GameScene* scene);
    static cocos2d::Scene* createPhysicsScene(GameScene* scene, cocos2d::Vec2 gravity = cocos2d::Vec2{}, bool autoStep = false);

    virtual bool init() override;
    virtual void resumeScene();
    virtual void pauseScene();
    virtual void quit();

    bool residesInSceneBounds(const cocos2d::Node& node) const;

    const cocos2d::Rect& sceneFrame() const {
        return frame;
    };

  protected:
    cocos2d::Rect frame;
};

}
