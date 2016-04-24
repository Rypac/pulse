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
    setMirrorsVisible(false);
    horizontalMirror->setColor(Color3B::RED);
    verticalMirror->setColor(Color3B::GREEN);
    diagonalMirror->setColor(Color3B::GREEN);
    addChild(horizontalMirror);
    addChild(verticalMirror);
    addChild(diagonalMirror);

    return true;
}

float normalisedHoriztonalPosition(float x, const Rect& rect, const Rect& bounds) {
    if (rect.getMinX() > bounds.getMaxX()) {
        return x - bounds.size.width;
    } else if (rect.getMaxX() < bounds.getMinX()) {
        return x + bounds.size.width;
    }
    return x;
}

float normalisedVerticalPosition(float y, const Rect& rect, const Rect& bounds) {
    if (rect.getMinY() > bounds.getMaxY()) {
        return y - bounds.size.height;
    } else if (rect.getMaxY() < bounds.getMinY()) {
        return y + bounds.size.height;
    }
    return y;
}

void WrappedSprite::normalisePosition(const Rect& bounds) {
    const auto frame = getBoundingBox();
    const auto x = normalisedHoriztonalPosition(frame.getMidX(), frame, bounds);
    const auto y = normalisedVerticalPosition(frame.getMidY(), frame, bounds);
    Sprite::setPosition(x, y);
}

void WrappedSprite::setPosition(float x, float y) {
    Sprite::setPosition(x, y);
    const auto parent = getParent();
    if (!parent) {
        return;
    }

    normalisePosition(parent->getBoundingBox());
}

void WrappedSprite::setMirrorsPosition(float x, float y) {
    const auto size = getContentSize();
    const auto position = convertToNodeSpace(Vec2{x, y});
    const auto parent = getParent();
    const auto parentFrame = parent->getBoundingBox();
    const auto parentCenter = Vec2{parentFrame.getMidX(), parentFrame.getMidY()};
    const auto center = convertToNodeSpace(parentCenter);
    const auto horizontalDiff = position.x - center.x;
    const auto diff = (size.width + x > parentFrame.getMaxX()) ? 1 : -1;
    const auto horizontalPosition = Vec2{position.x + parentFrame.size.width * diff, position.y};
    horizontalMirror->setPosition(horizontalPosition);

    const auto verticalDiff = position.y - center.y;
    const auto verticalPosition = Vec2{position.x, center.y - verticalDiff - size.height};
    verticalMirror->setPosition(verticalPosition);

    const auto diagonalPosition = Vec2{center.x - horizontalDiff, center.y - verticalDiff};
    diagonalMirror->setPosition(diagonalPosition);
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
        setMirrorsVisible(false);
    }
}

void WrappedSprite::setMirrorsVisible(bool visible) {
    horizontalMirror->setVisible(visible);
    verticalMirror->setVisible(visible);
    diagonalMirror->setVisible(visible);
}
