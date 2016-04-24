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
    if (sprite && sprite->init()) {
        sprite->autorelease();
        return sprite;
    }
    delete sprite;
    return nullptr;
}

bool WrappedSprite::init() {
    horizontalMirror = Sprite::create();
    verticalMirror = Sprite::create();
    diagonalMirror = Sprite::create();

    if (!Sprite::init()) {
        return false;
    }

    setColor(Color3B::WHITE);
    horizontalMirror->setColor(Color3B::RED);
    verticalMirror->setColor(Color3B::GREEN);
    diagonalMirror->setColor(Color3B::GREEN);

    horizontalMirror->setVisible(true);
    verticalMirror->setVisible(false);
    diagonalMirror->setVisible(false);

    return true;
}

bool WrappedSprite::initWithFile(const std::string &filename) {
    horizontalMirror = Sprite::create(filename);
    verticalMirror = Sprite::create(filename);
    diagonalMirror = Sprite::create(filename);

    if (!Sprite::initWithFile(filename)) {
        return false;
    }

    horizontalMirror->setVisible(true);
    verticalMirror->setVisible(false);
    diagonalMirror->setVisible(false);

    return true;
}

void WrappedSprite::setPosition(float x, float y) {
//    log("Setting position");
    Sprite::setPosition(x, y);
//    horizontalMirror->setPosition(x + 30, y + 30);
    const auto parent = getParent();
    if (!parent) {
        return;
    }

    const auto position = Vec2{x, y};
    const auto worldPosition = convertToWorldSpace(position);
    const auto parentFrame = parent->getBoundingBox();
    const auto parentCenter = Vec2{parentFrame.getMidX(), parentFrame.getMidY()};
    auto center = convertToNodeSpace(parentCenter);
    const auto horizontalDiff = worldPosition.x - center.x;
    const auto horizontalPosition = Vec2{center.x + horizontalDiff, worldPosition.y};
    horizontalMirror->setPosition(horizontalPosition);
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

void WrappedSprite::setParent(cocos2d::Node* parent) {
    Sprite::setParent(parent);
    if (parent) {
        parent->addChild(horizontalMirror, getLocalZOrder());
        parent->addChild(verticalMirror, getLocalZOrder());
        parent->addChild(diagonalMirror, getLocalZOrder());
    } else {
        horizontalMirror->removeFromParent();
        verticalMirror->removeFromParent();
        diagonalMirror->removeFromParent();
    }
}

void WrappedSprite::removeFromParentAndCleanup(bool cleanup) {
    Sprite::removeFromParentAndCleanup(cleanup);
    horizontalMirror->removeFromParentAndCleanup(cleanup);
    verticalMirror->removeFromParentAndCleanup(cleanup);
    diagonalMirror->removeFromParentAndCleanup(cleanup);
}

void WrappedSprite::removeAllChildrenWithCleanup(bool cleanup) {
    Sprite::removeAllChildrenWithCleanup(cleanup);
    horizontalMirror->removeAllChildrenWithCleanup(cleanup);
    verticalMirror->removeAllChildrenWithCleanup(cleanup);
    diagonalMirror->removeAllChildrenWithCleanup(cleanup);
}
