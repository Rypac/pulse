#include "pulse/sprites/MirrorSprite.hpp"

using namespace pulse;
using namespace cocos2d;

MirrorSprite* MirrorSprite::createWithTexture(Texture2D *texture, const Rect& rect, bool rotated) {
    MirrorSprite *sprite = new (std::nothrow) MirrorSprite();
    if (sprite && sprite->initWithTexture(texture, rect, rotated)) {
        sprite->autorelease();
        return sprite;
    }
    delete sprite;
    return nullptr;
}

void MirrorSprite::setPhysicsBody(PhysicsBody *physicsBody) {
    Sprite::setPhysicsBody(physicsBody);
    physicsBody->setEnabled(isVisible());
}

void MirrorSprite::setVisible(bool visible) {
    Sprite::setVisible(visible);
    if (getPhysicsBody()) {
        getPhysicsBody()->setEnabled(visible);
    }
}
