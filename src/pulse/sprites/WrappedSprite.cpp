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

    setMirrorsVisible(false);
    setColor(Color3B::WHITE);
    horizontalMirror->setColor(Color3B::WHITE);
    verticalMirror->setColor(Color3B::WHITE);
    diagonalMirror->setColor(Color3B::WHITE);
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

float mirroredHoriztonalPosition(float x, const Rect& rect, const Rect& bounds) {
    if (rect.getMaxX() > bounds.getMaxX()) {
        return x - bounds.size.width;
    } else if (rect.getMinX() < bounds.getMinX()) {
        return x + bounds.size.width;
    }
    return x;
}

float mirroredVerticalPosition(float y, const Rect& rect, const Rect& bounds) {
    if (rect.getMaxY() > bounds.getMaxY()) {
        return y - bounds.size.height;
    } else if (rect.getMinY() < bounds.getMinY()) {
        return y + bounds.size.height;
    }
    return y;
}

bool exceedsHorizontalBounds(const Rect& rect, const Rect& bounds) {
    return rect.getMaxX() > bounds.getMaxX() || rect.getMinX() < bounds.getMinX();
}

bool exceedsVerticalBounds(const Rect& rect, const Rect& bounds) {
    return rect.getMaxY() > bounds.getMaxY() || rect.getMinY() < bounds.getMinY();
}

bool exceedsBounds(const Rect& rect, const Rect& bounds) {
    return exceedsVerticalBounds(rect, bounds) || exceedsHorizontalBounds(rect, bounds);
}

bool withinHorizontalBounds(const Rect& rect, const Rect& bounds) {
    return rect.getMinX() > bounds.getMinX() && rect.getMaxX() < bounds.getMaxX();
}

bool withinVerticalBounds(const Rect& rect, const Rect& bounds) {
    return rect.getMinY() > bounds.getMinY() && rect.getMaxY() < bounds.getMaxY();
}

bool withinBounds(const Rect& rect, const Rect& bounds) {
    return withinHorizontalBounds(rect, bounds) && withinVerticalBounds(rect, bounds);
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

    const auto position = convertToWorldSpace(getPosition());
    const auto frame = Rect{position, getContentSize()};
    const auto bounds = parent->getBoundingBox();
    if (!withinBounds(frame, bounds)) {
        normalisePosition(bounds);
        drawMirrors(bounds);
    } else {
        setMirrorsVisible(false);
    }
}

void WrappedSprite::drawMirrors(const Rect& bounds) {
    const auto frame = getBoundingBox();
    const auto mirrorX = mirroredHoriztonalPosition(frame.getMidX(), frame, bounds);
    const auto mirrorY = mirroredVerticalPosition(frame.getMidY(), frame, bounds);

    const auto convertedPosition = convertToNodeSpace(getPosition());
    const auto mirroredPosition = convertToNodeSpace(Vec2{mirrorX, mirrorY});
    horizontalMirror->setPosition(mirroredPosition.x, convertedPosition.y);
    verticalMirror->setPosition(convertedPosition.x, mirroredPosition.y);
    diagonalMirror->setPosition(mirroredPosition.x, mirroredPosition.y);

    horizontalMirror->setVisible(exceedsHorizontalBounds(frame, bounds));
    verticalMirror->setVisible(exceedsVerticalBounds(frame, bounds));
    diagonalMirror->setVisible(horizontalMirror->isVisible() && verticalMirror->isVisible());
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
    setMirrorsVisible(visible);
}

void WrappedSprite::setMirrorsVisible(bool visible) {
    horizontalMirror->setVisible(visible);
    verticalMirror->setVisible(visible);
    diagonalMirror->setVisible(visible);
}
