#pragma once

#include "cocos2d.h"

namespace pulse {

class MirrorSprite : public cocos2d::Sprite {
  public:
    static MirrorSprite* createWithTexture(cocos2d::Texture2D* texture, const cocos2d::Rect& rect, bool rotated = false);

    virtual void setVisible(bool visible) override;
    void setPhysicsBody(cocos2d::PhysicsBody* physicsBody);
};

}  // pulse
