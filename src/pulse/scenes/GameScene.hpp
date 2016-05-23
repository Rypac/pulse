#pragma once

#include "cocos2d.h"

namespace pulse {

class GameScene : public cocos2d::Layer {
  public:
    static cocos2d::Scene* createScene(GameScene* scene);

    virtual void resumeScene();
    virtual void pauseScene();
    virtual void quit();

    virtual cocos2d::Node* background() const;
    virtual void setBackground(cocos2d::Node* background);
    virtual void removeBackground();

    bool residesInSceneBounds(const cocos2d::Node& node) const;

    const cocos2d::Rect& sceneFrame() const {
        return frame_;
    };

  protected:
    GameScene();
    virtual ~GameScene();

    virtual bool init() override;

    cocos2d::Rect frame_;

  private:
    cocos2d::Node* background_;
};

}
