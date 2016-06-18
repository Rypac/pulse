#include "pulse/sprites/WrappedSprite.hpp"
#include "pulse/2d/Position.hpp"
#include "pulse/extensions/Rect.hpp"
#include "pulse/sprites/SpritePhysicsBody.hpp"
#include "pulse/ui/Resources.hpp"

using pulse::WrappedSprite;
using namespace cocos2d;

WrappedSprite* WrappedSprite::create() {
    const auto sprite = new (std::nothrow) WrappedSprite();
    if (sprite && sprite->init()) {
        sprite->autorelease();
        return sprite;
    }
    delete sprite;
    return nullptr;
}

WrappedSprite* WrappedSprite::create(const std::string& filename) {
    const auto sprite = new (std::nothrow) WrappedSprite();
    if (sprite && sprite->initWithFile(filename)) {
        sprite->autorelease();
        return sprite;
    }
    delete sprite;
    return nullptr;
}

WrappedSprite::~WrappedSprite() {
    CC_SAFE_RELEASE(horizontalMirror);
    CC_SAFE_RELEASE(verticalMirror);
    CC_SAFE_RELEASE(diagonalMirror);
}

bool WrappedSprite::initMirrors(Texture2D* texture, const Rect& rect, bool rotated) {
    horizontalMirror = MirrorSprite::createWithTexture(texture, rect, rotated);
    verticalMirror = MirrorSprite::createWithTexture(texture, rect, rotated);
    diagonalMirror = MirrorSprite::createWithTexture(texture, rect, rotated);
    horizontalMirror->retain();
    verticalMirror->retain();
    diagonalMirror->retain();
    return horizontalMirror != nullptr && verticalMirror != nullptr && diagonalMirror != nullptr;
}

bool WrappedSprite::initWithTexture(Texture2D* texture, const Rect& rect, bool rotated) {
    if (!(initMirrors(texture, rect, rotated) && Sprite::initWithTexture(texture, rect, rotated))) {
        return false;
    }

    setColor(Color3B::WHITE);
    applyToMirrors([this](auto mirror) {
        mirror->setVisible(false);
        mirror->setColor(Color3B::WHITE);
        this->addChild(mirror);
    });

    emitter = ParticleSystemQuad::create(Resources::Particles::PlayerTrail);
    addChild(emitter, -1);

    return true;
}

Vec2 WrappedSprite::relativeToAnchorPoint(const Vec2& position) const {
    return position::relativeToAnchorPoint(position, getContentSize(), getAnchorPoint());
}

void WrappedSprite::setPosition(const Vec2& position) {
    setPosition(position.x, position.y);
}

void WrappedSprite::setPosition(float x, float y) {
    Sprite::setPosition(x, y);
    emitter->setPosition(convertToNodeSpace(getPosition()));

    const auto parent = getParent();
    if (parent != nullptr) {
        normalisePositionInBounds(parent->getBoundingBox());
        return;
    }
}

void WrappedSprite::normalisePositionInBounds(const Rect& bounds) {
    const auto frame = getBoundingBox();
    const auto position = relativeToAnchorPoint(position::normalised(frame, bounds));
    Sprite::setPosition(position.x, position.y);

    const auto mirror = relativeToAnchorPoint(position::mirrored(frame, bounds));
    const auto childPosition = convertToNodeSpace(getPosition());
    const auto childMirror = convertToNodeSpace(mirror);
    horizontalMirror->setPosition(childMirror.x, childPosition.y);
    verticalMirror->setPosition(childPosition.x, childMirror.y);
    diagonalMirror->setPosition(childMirror.x, childMirror.y);

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

void WrappedSprite::setPhysicsBody(PhysicsBody* physicsBody) {
    Sprite::setPhysicsBody(physicsBody);
    applyToMirrors([&](auto mirror) {
        mirror->setPhysicsBody(physics_body::clone(physicsBody, mirror->getContentSize()));
    });
}

void WrappedSprite::applyToMirrors(const std::function<void(cocos2d::Sprite* mirror)> func) {
    func(horizontalMirror);
    func(verticalMirror);
    func(diagonalMirror);
}
