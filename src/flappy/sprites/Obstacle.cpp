#include "flappy/sprites/Obstacle.hpp"
#include "flappy/utilities/Geometry.hpp"

using namespace cocos2d;
using namespace flappy;

bool Obstacle::init() {
    if (!Sprite::init()) {
        return false;
    }

    top = Column::create();
    bottom = Column::create();
    addChild(top);
    addChild(bottom);
    setColor(Color3B::GREEN);

    return true;
}

Obstacle* Obstacle::create(Size size, Direction direction) {
    const auto obstacle = Obstacle::create();
    obstacle->setContentSize(size);
    obstacle->direction = direction;
    return obstacle;
}

Vec2 relativePosition(const Vec2& position, const Size& size, const Vec2& anchorPoint) {
    return Vec2{position.x + size.width * anchorPoint.x, position.y + size.height * anchorPoint.y};
}

Rect relativeRect(const Rect& frame, const Vec2& anchorPoint) {
    return Rect{relativePosition(frame.origin, frame.size, anchorPoint), frame.size};
}

void setFrameForTextureSprite(Sprite* sprite, Rect frame) {
    frame = relativeRect(frame, sprite->getAnchorPoint());
    sprite->setPosition(frame.origin);
    sprite->setContentSize(frame.size);
    sprite->setTextureRect(frame);
}

void setRotationForDirection(Obstacle* obstacle) {
    if (obstacle->getDirection() == Direction::North || obstacle->getDirection() == Direction::South) {
        obstacle->setRotation(90);
    }
}

Obstacle* Obstacle::create(float topLength, float gapLength, float bottomLength, Direction direction) {
    const auto obstacleSize = Size{Column::defaultLength, topLength + gapLength + bottomLength};
    const auto bottomFrame = Rect{0, 0, obstacleSize.width, bottomLength};
    const auto topFrame = Rect{0, bottomLength + gapLength, obstacleSize.width, topLength};
    const auto obstacle = Obstacle::create(obstacleSize, direction);
    obstacle->setTextureRect(obstacle->getBoundingBox());
    setFrameForTextureSprite(obstacle->bottom, bottomFrame);
    setFrameForTextureSprite(obstacle->top, topFrame);
    setRotationForDirection(obstacle);
    return obstacle;
}

void Obstacle::positionInWorld(cocos2d::Rect world) {
    const auto body = getBoundingBox();
    const auto origin = geometry::origin(body, world, direction);
    const auto final = geometry::destination(body, world, direction);
    setPosition(relativePosition(origin, body.size, getAnchorPoint()));
    destination = relativePosition(final, body.size, getAnchorPoint());
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

Vec2 worldPosition(const Node& node) {
    return node.getParent()->convertToWorldSpace(node.getPosition());
}

Rect worldRect(const Node& node) {
    return Rect{worldPosition(node), node.getBoundingBox().size};
}

bool Obstacle::collidesWith(cocos2d::Rect frame) const {
    return frame.intersectsRect(worldRect(*top)) || frame.intersectsRect(worldRect(*bottom));
}

bool Obstacle::passedBy(cocos2d::Rect frame) const {
    return frame.getMinX() > getBoundingBox().getMaxX();
}
