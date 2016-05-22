#pragma once

#include "cocos2d.h"
#include "pulse/sprites/MirrorSprite.hpp"

namespace pulse {

class WrappedSprite : public cocos2d::Sprite {
  public:
    static WrappedSprite* create();
    static WrappedSprite* create(const std::string& filename);

    virtual void setPosition(float x, float y) override;
    virtual void setPosition(const cocos2d::Vec2& position) override;
    virtual void setContentSize(const cocos2d::Size& size) override;
    virtual void setTextureRect(const cocos2d::Rect& rect) override;

    void normalisePositionInBounds(const cocos2d::Rect& bounds);

    void setPhysicsBody(cocos2d::PhysicsBody* physicsBody);

    cocos2d::Vec2 relativeToAnchorPoint(const cocos2d::Vec2& position) const;

  protected:
    WrappedSprite() {}
    virtual ~WrappedSprite();

    virtual bool initMirrors(cocos2d::Texture2D* texture, const cocos2d::Rect& rect, bool rotated);
    virtual bool initWithTexture(cocos2d::Texture2D* texture, const cocos2d::Rect& rect, bool rotated) override;

  private:
    void applyToMirrors(const std::function<void (cocos2d::Sprite* mirror)> func);

    cocos2d::ParticleSystemQuad* emitter;
    MirrorSprite* horizontalMirror;
    MirrorSprite* verticalMirror;
    MirrorSprite* diagonalMirror;
};

}
