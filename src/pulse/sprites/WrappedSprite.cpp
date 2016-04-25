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
    applyToMirrors([this](auto mirror) {
        mirror->setVisible(false);
        mirror->setColor(Color3B::WHITE);
        addChild(mirror);
    });

    return true;
}

float normalisedHoriztonalPosition(const Rect& rect, const Rect& bounds) {
    if (rect.getMinX() > bounds.getMaxX()) {
        return rect.getMinX() - bounds.size.width;
    } else if (rect.getMaxX() < bounds.getMinX()) {
        return rect.getMinX() + bounds.size.width;
    }
    return rect.getMinX();
}

float normalisedVerticalPosition(const Rect& rect, const Rect& bounds) {
    if (rect.getMinY() > bounds.getMaxY()) {
        return rect.getMinY() - bounds.size.height;
    } else if (rect.getMaxY() < bounds.getMinY()) {
        return rect.getMinY() + bounds.size.height;
    }
    return rect.getMinY();
}

Vec2 normalisedPosition(const Rect& body, const Rect& bounds) {
    return Vec2{normalisedHoriztonalPosition(body, bounds), normalisedVerticalPosition(body, bounds)};
}

float mirroredHoriztonalPosition(const Rect& rect, const Rect& bounds) {
    if (rect.getMaxX() > bounds.getMaxX()) {
        return rect.getMinX() - bounds.size.width;
    } else if (rect.getMinX() < bounds.getMinX()) {
        return rect.getMinX() + bounds.size.width;
    }
    return rect.getMinX();
}

float mirroredVerticalPosition(const Rect& rect, const Rect& bounds) {
    if (rect.getMaxY() > bounds.getMaxY()) {
        return rect.getMinY() - bounds.size.height;
    } else if (rect.getMinY() < bounds.getMinY()) {
        return rect.getMinY() + bounds.size.height;
    }
    return rect.getMinY();
}

Vec2 mirroredPosition(const Rect& body, const Rect& bounds) {
    return Vec2{mirroredHoriztonalPosition(body, bounds), mirroredVerticalPosition(body, bounds)};
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
    const auto frame = Rect{getPosition(), getBoundingBox().size};
    const auto x = normalisedHoriztonalPosition(frame, bounds);
    const auto y = normalisedVerticalPosition(frame, bounds);
    Sprite::setPosition(x, y);
}

void WrappedSprite::setPosition(float x, float y) {
    Sprite::setPosition(x, y);
    const auto parent = getParent();
    if (!parent) {
        return;
    }

    const auto frame = Rect{convertToWorldSpace(getPosition()), getBoundingBox().size};
    const auto bounds = parent->getBoundingBox();
    if (!withinBounds(frame, bounds)) {
        normalisePosition(bounds);
        drawMirrors(bounds);
    } else {
        setMirrorsVisible(false);
    }
}

void WrappedSprite::drawMirrors(const Rect& bounds) {
    const auto position = getPosition();
    const auto frame = Rect{position, getBoundingBox().size};
    auto mirror = convertToNodeSpace(mirroredPosition(frame, bounds));
    horizontalMirror->setPosition(mirror.x, position.y);
    verticalMirror->setPosition(position.x, mirror.y);
    diagonalMirror->setPosition(mirror.x, mirror.y);

    horizontalMirror->setVisible(exceedsHorizontalBounds(frame, bounds));
    verticalMirror->setVisible(exceedsVerticalBounds(frame, bounds));
    diagonalMirror->setVisible(horizontalMirror->isVisible() && verticalMirror->isVisible());
}

void WrappedSprite::setContentSize(const Size& size) {
    Sprite::setContentSize(size);
    applyToMirrors([&](auto mirror) { mirror->setContentSize(size); });
}

void WrappedSprite::setTextureRect(const Rect& rect) {
    Sprite::setTextureRect(rect);
    applyToMirrors([&](auto mirror) { mirror->setTextureRect(rect); });
}

void WrappedSprite::setPhysicsBody(Component *physicsBody) {
    Sprite::setPhysicsBody(physicsBody);
    applyToMirrors([&](auto mirror) { mirror->setPhysicsBody(physicsBody); });
}

void WrappedSprite::setVisible(bool visible) {
    Sprite::setVisible(visible);
    setMirrorsVisible(visible);
}

void WrappedSprite::setMirrorsVisible(bool visible) {
    applyToMirrors([&](auto mirror) { mirror->setVisible(visible); });
}

void WrappedSprite::applyToMirrors(const std::function<void (Sprite *)> func) {
    func(horizontalMirror);
    func(verticalMirror);
    func(diagonalMirror);
}
