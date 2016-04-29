#include "pulse/sprites/WrappedSprite.hpp"
#include "pulse/sprites/SpritePhysicsBody.hpp"
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

Vec2 WrappedSprite::relativeToAnchorPoint(const Vec2& position) const {
    return position::relativeToAnchorPoint(position, getContentSize(), getAnchorPoint());
}

void WrappedSprite::normalisePosition(const Rect& bounds) {
    const auto position = position::normalised(getBoundingBox(), bounds);
    const auto relativePosition = relativeToAnchorPoint(position);
    Sprite::setPosition(relativePosition.x, relativePosition.y);
}

void WrappedSprite::setPosition(const Vec2& position) {
    setPosition(position.x, position.y);
}

void WrappedSprite::setPosition(float x, float y) {
    Sprite::setPosition(x, y);
    const auto parent = getParent();
    if (!parent) {
        return;
    }

    const auto frame = getBoundingBox();
    const auto bounds = parent->getBoundingBox();
    if (!rect::withinBounds(frame, bounds)) {
        normalisePosition(bounds);
        drawMirrors(bounds);
    } else {
        setMirrorsVisible(false);
    }
}

void WrappedSprite::drawMirrors(const Rect& bounds) {
    const auto frame = getBoundingBox();
    const auto mirror = relativeToAnchorPoint(position::mirrored(frame, bounds));
    const auto childPosition = convertToNodeSpace(getPosition());
    const auto childMirror = convertToNodeSpace(mirror);
    horizontalMirror->setPosition(childMirror.x, childPosition.y);
    verticalMirror->setPosition(childPosition.x, childMirror.y);
    diagonalMirror->setPosition(childMirror.x, childMirror.y);

    horizontalMirror->setVisible(rect::exceedsHorizontalBounds(frame, bounds));
    verticalMirror->setVisible(rect::exceedsVerticalBounds(frame, bounds));
    diagonalMirror->setVisible(horizontalMirror->isVisible() && verticalMirror->isVisible());
    applyToMirrors([](auto mirror) {
        if (mirror->getPhysicsBody()) {
            mirror->getPhysicsBody()->setEnabled(mirror->isVisible());
        }
    });
}

void WrappedSprite::setContentSize(const Size& size) {
    Sprite::setContentSize(size);
    applyToMirrors([&](auto mirror) { mirror->setContentSize(size); });
}

void WrappedSprite::setTextureRect(const Rect& rect) {
    Sprite::setTextureRect(rect);
    applyToMirrors([&](auto mirror) { mirror->setTextureRect(rect); });
}

void WrappedSprite::setPhysicsBody(PhysicsBody *physicsBody) {
    Sprite::setPhysicsBody(physicsBody);
    applyToMirrors([&](auto mirror) {
        const auto body = physics_body::clone(physicsBody, mirror->getContentSize());
        mirror->setPhysicsBody(body);
    });
}

void WrappedSprite::setVisible(bool visible) {
    Sprite::setVisible(visible);
    setMirrorsVisible(visible);
}

void WrappedSprite::setMirrorsVisible(bool visible) {
    applyToMirrors([&](auto mirror) {
        mirror->setVisible(visible);
        if (mirror->getPhysicsBody()) {
            mirror->getPhysicsBody()->setEnabled(visible);
        }
    });
}

void WrappedSprite::applyToMirrors(const std::function<void (Sprite *)> func) {
    func(horizontalMirror);
    func(verticalMirror);
    func(diagonalMirror);
}
