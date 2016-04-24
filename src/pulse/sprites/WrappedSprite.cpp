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
    addChild(horizontalMirror);
    addChild(verticalMirror);
    addChild(diagonalMirror);

    return true;
}

void WrappedSprite::setPosition(float x, float y) {
    Sprite::setPosition(x, y);
    const auto parent = getParent();
    if (!parent) {
        return;
    }

    const auto position = convertToNodeSpace(Vec2{x, y});
    const auto parentFrame = parent->getBoundingBox();
    const auto parentCenter = Vec2{parentFrame.getMidX(), parentFrame.getMidY()};
    auto center = convertToNodeSpace(parentCenter);
    const auto horizontalDiff = position.x - center.x;
    const auto horizontalPosition = Vec2{center.x - horizontalDiff, position.y};
    horizontalMirror->setPosition(horizontalPosition);

    const auto verticalDiff = position.y - center.y;
    const auto verticalPosition = Vec2{position.x, center.y - verticalDiff};
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
        horizontalMirror->setVisible(false);
        verticalMirror->setVisible(false);
        diagonalMirror->setVisible(false);
    }
}
