#pragma once

#include "cocos2d.h"

namespace pulse {

class GameScene : public cocos2d::Layer {
  public:
    static cocos2d::Scene* createScene(GameScene* scene);

    virtual void resumeScene();
    virtual void pauseScene();
    virtual void quit();

    bool residesInSceneBounds(const cocos2d::Node& node) const;

    const cocos2d::Rect& sceneFrame() const {
        return frame;
    };

  protected:
    virtual bool init() override;

    cocos2d::Rect frame;
};

}
