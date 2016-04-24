#include "pulse/sprites/WrappedSprite.hpp"

using pulse::WrappedSprite;
using cocos2d::Sprite;

WrappedSprite* WrappedSprite::create() {
    WrappedSprite *sprite = new (std::nothrow) WrappedSprite();
    if (sprite && sprite->init()) {
        sprite->autorelease();
        return sprite;
    }
    delete sprite;
    return nullptr;
}

WrappedSprite* WrappedSprite::create(const std::string& filename) {
    WrappedSprite *sprite = new (std::nothrow) WrappedSprite();
    if (sprite && sprite->init()) {
        sprite->autorelease();
        return sprite;
    }
    delete sprite;
    return nullptr;
}

bool WrappedSprite::init() {
    if (!Sprite::init()) {
        return false;
    }

    return true;
}

bool WrappedSprite::initWithFile(const std::string &filename) {
    if (!Sprite::initWithFile(filename)) {
        return false;
    }

    horizontalMirror = Sprite::create(filename);
    verticalMirror = Sprite::create(filename);
    diagonalMirror = Sprite::create(filename);
    horizontalMirror->setVisible(false);
    verticalMirror->setVisible(false);
    diagonalMirror->setVisible(false);

    return true;
}

void WrappedSprite::setContentSize(const cocos2d::Size& size) {
    Sprite::setContentSize(size);
    horizontalMirror->setContentSize(size);
    verticalMirror->setContentSize(size);
    diagonalMirror->setContentSize(size);
}

void WrappedSprite::setVisible(bool visible) {
    Sprite::setVisible(visible);
    if (!visible) {
        horizontalMirror->setVisible(false);
        verticalMirror->setVisible(false);
        diagonalMirror->setVisible(false);
    }
}

void WrappedSprite::setParent(cocos2d::Node* parent) {
    Sprite::setParent(parent);
    horizontalMirror->setParent(parent);
    verticalMirror->setParent(parent);
    diagonalMirror->setParent(parent);
}

void WrappedSprite::removeFromParent() {
    Sprite::removeFromParent();
    horizontalMirror->removeFromParent();
    verticalMirror->removeFromParent();
    diagonalMirror->removeFromParent();
}

void WrappedSprite::removeFromParentAndCleanup(bool cleanup) {
    Sprite::removeFromParentAndCleanup(cleanup);
    horizontalMirror->removeFromParentAndCleanup(cleanup);
    verticalMirror->removeFromParentAndCleanup(cleanup);
    diagonalMirror->removeFromParentAndCleanup(cleanup);
}

void WrappedSprite::update(float dt) {
    Sprite::update(dt);
}
