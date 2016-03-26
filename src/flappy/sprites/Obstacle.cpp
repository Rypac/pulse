#include "Obstacle.hpp"

using namespace cocos2d;
using namespace flappy;

bool Obstacle::init() {
    if (!Sprite::init()) {
        return false;
    }

    direction = Direction::West;
    top = Column::create();
    bottom = Column::create();
    addChild(top);
    addChild(bottom);
    setAnchorPoint(Vec2{0, 0});

    return true;
}

Obstacle* Obstacle::create(Size frame) {
    const auto obstacle = Obstacle::create();
    obstacle->setContentSize(frame);
    return obstacle;
}

void setFrameForTextureSprite(Sprite* sprite, Rect frame) {
    sprite->setPosition(frame.origin);
    sprite->setContentSize(frame.size);
    sprite->setTextureRect(frame);
}

Obstacle* Obstacle::create(float topColumnHeight, float gapHeight, float bottomColumnHeight) {
    const auto obstacleSize = Size{Column::defaultWidth, topColumnHeight + gapHeight + bottomColumnHeight};
    const auto bottomFrame = Rect{0, 0, obstacleSize.width, bottomColumnHeight};
    const auto topFrame = Rect{0, bottomColumnHeight + gapHeight, obstacleSize.width, topColumnHeight};
    const auto obstacle = Obstacle::create(obstacleSize);
    setFrameForTextureSprite(obstacle->bottom, bottomFrame);
    setFrameForTextureSprite(obstacle->top, topFrame);
    return obstacle;
}

Vec2 Obstacle::origin(Rect world) const {
    const auto body = getBoundingBox();
    switch (direction) {
        case Direction::North: return Vec2{body.origin.x, world.origin.y - body.size.height};
        case Direction::South: return Vec2{body.origin.x, world.origin.y + world.size.height + body.size.height};
        case Direction::East: return Vec2{world.origin.x - body.size.width, body.origin.y};
        case Direction::West: return Vec2{world.origin.x + world.size.width + body.size.width, body.origin.y};
    }
}

Vec2 Obstacle::destination(Rect world) const {
    const auto body = getBoundingBox();
    switch (direction) {
        case Direction::North: return Vec2{body.origin.x, world.origin.y + world.size.height + body.size.height};
        case Direction::South: return Vec2{body.origin.x, world.origin.y - body.size.height};
        case Direction::East: return Vec2{world.origin.x + world.size.width + body.size.width, body.origin.y};
        case Direction::West: return Vec2{world.origin.x - body.size.width, body.origin.y};
    }
}

void Obstacle::runActions(Rect world, ObstacleCallback onCompletion) {
    const auto moveToEdge = MoveTo::create(5, destination(world));
    const auto removeFromScene = RemoveSelf::create(true);
    const auto actionsCompleted = CallFunc::create([=]() {
        if (onCompletion) {
            onCompletion(this);
        }
    });
    const auto actions = Sequence::create(moveToEdge, removeFromScene, actionsCompleted, nullptr);
    runAction(actions);
}

void Obstacle::positionInWorld(cocos2d::Rect world) {
    setPosition(origin(world));
}

Rect frameInParentSpace(const Sprite* child, const Sprite* parent) {
    const auto childFrame = child->getBoundingBox();
    return Rect(parent->convertToWorldSpace(childFrame.origin), childFrame.size);
}

bool Obstacle::collidesWith(cocos2d::Rect frame) const {
    return frame.intersectsRect(frameInParentSpace(top, this))
        || frame.intersectsRect(frameInParentSpace(bottom, this));
}

bool Obstacle::passedBy(cocos2d::Rect frame) const {
    return frame.getMinX() > getBoundingBox().getMaxX();
}
