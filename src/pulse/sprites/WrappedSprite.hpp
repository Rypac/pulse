#pragma once

#include "cocos2d.h"
#include <string>

namespace pulse {

class WrappedSprite : public cocos2d::Sprite {
  public:
    static WrappedSprite* create();
    static WrappedSprite* create(const std::string& filename);

    virtual bool initWithTexture(cocos2d::Texture2D *texture, const cocos2d::Rect& rect, bool rotated) override;

    virtual void setVisible(bool visible) override;
    virtual void setPosition(float x, float y) override;
    virtual void setContentSize(const cocos2d::Size& size) override;
    virtual void setTextureRect(const cocos2d::Rect& rect) override;

    void setPhysicsBody(cocos2d::Component* physicsBody);

    cocos2d::Vec2 relativeToAnchorPoint(const cocos2d::Vec2& position) const;

  private:
    void applyToMirrors(const std::function<void (cocos2d::Sprite* mirror)> func);
    void normalisePosition(const cocos2d::Rect& bounds);
    void setMirrorsVisible(bool visible);
    void drawMirrors(const cocos2d::Rect& bounds);

    cocos2d::Sprite* horizontalMirror;
    cocos2d::Sprite* verticalMirror;
    cocos2d::Sprite* diagonalMirror;
};

}
