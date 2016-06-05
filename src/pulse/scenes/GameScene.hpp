#pragma once

#include "cocos2d.h"

namespace pulse {

class GameScene : public cocos2d::Scene {
  public:
    GameScene();
    virtual ~GameScene();

    virtual void resumeScene();
    virtual void pauseScene();
    virtual void quit();

    virtual void setBackground(cocos2d::Node* background);
    virtual void removeBackground();

    bool residesInSceneBounds(const cocos2d::Node& node) const;

    const cocos2d::Rect& sceneFrame() const {
        return frame_;
    }

    cocos2d::Node* background() const {
        return background_;
    }

  private:
    cocos2d::Rect frame_;
    cocos2d::Node* background_;
};

}
