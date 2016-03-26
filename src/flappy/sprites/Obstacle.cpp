#include "flappy/sprites/Obstacle.hpp"
#include "flappy/utilities/Geometry.hpp"

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

void Obstacle::positionInWorld(cocos2d::Rect world) {
    const auto body = getBoundingBox();
    setPosition(geometry::origin(body, world, direction));
    destination = geometry::destination(body, world, direction);
}

void Obstacle::runActions(ObstacleCallback onCompletion) {
    const auto moveToEdge = MoveTo::create(5, destination);
    const auto removeFromScene = RemoveSelf::create(true);
    const auto actionsCompleted = CallFunc::create([=]() {
        if (onCompletion) {
            onCompletion(this);
        }
    });
    const auto actions = Sequence::create(moveToEdge, removeFromScene, actionsCompleted, nullptr);
    runAction(actions);
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
