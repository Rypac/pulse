#pragma once

#include "cocos2d.h"
#include <string>

namespace pulse {

class WrappedSprite : public cocos2d::Sprite {
  public:
    static WrappedSprite* create();
    static WrappedSprite* create(const std::string& filename);

    virtual bool init() override;
    virtual bool initWithFile(const std::string& filename) override;

    virtual void setParent(cocos2d::Node* parent) override;
    virtual void setVisible(bool visible) override;
    virtual void setContentSize(const cocos2d::Size& size) override;

    virtual void removeFromParent() override;
    virtual void removeFromParentAndCleanup(bool cleanup) override;

    virtual void update(float dt) override;

  private:
    cocos2d::Sprite* horizontalMirror;
    cocos2d::Sprite* verticalMirror;
    cocos2d::Sprite* diagonalMirror;
};

}
