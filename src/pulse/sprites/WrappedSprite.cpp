#include "pulse/sprites/WrappedSprite.hpp"
#include "pulse/utilities/Position.hpp"
#include "pulse/utilities/Rect.hpp"

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

void WrappedSprite::normalisePosition(const Rect& bounds) {
    const auto frame = Rect{getPosition(), getBoundingBox().size};
    const auto x = position::normalisedHoriztonal(frame, bounds);
    const auto y = position::normalisedVertical(frame, bounds);
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
    if (!rect::withinBounds(frame, bounds)) {
        normalisePosition(bounds);
        drawMirrors(bounds);
    } else {
        setMirrorsVisible(false);
    }
}

void WrappedSprite::drawMirrors(const Rect& bounds) {
    const auto position = getPosition();
    const auto frame = Rect{position, getBoundingBox().size};
    auto mirror = convertToNodeSpace(position::mirrored(frame, bounds));
    horizontalMirror->setPosition(mirror.x, position.y);
    verticalMirror->setPosition(position.x, mirror.y);
    diagonalMirror->setPosition(mirror.x, mirror.y);

    horizontalMirror->setVisible(rect::exceedsHorizontalBounds(frame, bounds));
    verticalMirror->setVisible(rect::exceedsVerticalBounds(frame, bounds));
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
