#include "pulse/sprites/WrappedSprite.hpp"

using pulse::WrappedSprite;
using namespace cocos2d;

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
    if (sprite && sprite->initWithFile(filename)) {
        sprite->autorelease();
        return sprite;
    }
    delete sprite;
    return nullptr;
}

bool WrappedSprite::initWithTexture(Texture2D *texture, const Rect& rect, bool rotated) {
    horizontalMirror = Sprite::createWithTexture(texture, rect, rotated);
    verticalMirror = Sprite::createWithTexture(texture, rect, rotated);
    diagonalMirror = Sprite::createWithTexture(texture, rect, rotated);

    if (!Sprite::initWithTexture(texture, rect, rotated)) {
        return false;
    }

    setColor(Color3B::WHITE);
    horizontalMirror->setColor(Color3B::RED);
    verticalMirror->setColor(Color3B::GREEN);
    diagonalMirror->setColor(Color3B::GREEN);
    horizontalMirror->setVisible(false);
    verticalMirror->setVisible(false);
    diagonalMirror->setVisible(false);
    addChild(horizontalMirror);
    addChild(verticalMirror);
    addChild(diagonalMirror);

    return true;
}

float normalisedHoriztonalPosition(float x, const Rect& rect) {
    return x > rect.getMaxX() ? x - rect.size.width : x < rect.getMinX() ? x + rect.size.width : x;
}

float normalisedVerticalPosition(float y, const Rect& rect) {
    return y > rect.getMaxY() ? y - rect.size.height : y < rect.getMinY() ? y + rect.size.height : y;
}

void WrappedSprite::setPosition(float x, float y) {
    const auto parent = getParent();
    if (!parent) {
        Sprite::setPosition(x, y);
        return;
    }

    const auto size = getContentSize();
    const auto parentFrame = parent->getBoundingBox();
    const auto normalisedX = normalisedHoriztonalPosition(x, parentFrame);
    const auto normalisedY = normalisedVerticalPosition(y, parentFrame);
    Sprite::setPosition(normalisedX, normalisedY);
}

void WrappedSprite::setContentSize(const Size& size) {
    Sprite::setContentSize(size);
    horizontalMirror->setContentSize(size);
    verticalMirror->setContentSize(size);
    diagonalMirror->setContentSize(size);
}

void WrappedSprite::setTextureRect(const Rect& rect) {
    Sprite::setTextureRect(rect);
    horizontalMirror->setTextureRect(rect);
    verticalMirror->setTextureRect(rect);
    diagonalMirror->setTextureRect(rect);
}

void WrappedSprite::setVisible(bool visible) {
    Sprite::setVisible(visible);
    if (!visible) {
        horizontalMirror->setVisible(false);
        verticalMirror->setVisible(false);
        diagonalMirror->setVisible(false);
    }
}
