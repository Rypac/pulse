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
    obstacle->setPosition(0, 0);
    obstacle->setContentSize(size);
    obstacle->setTextureRect(obstacle->getBoundingBox());
    obstacle->direction = direction;
    return obstacle;
}

void setFrameForTextureSprite(Sprite* sprite, Vec2 position, Size size) {
    sprite->setPosition(position);
    sprite->setContentSize(size);
    sprite->setTextureRect(sprite->getBoundingBox());
}

void setRotationForDirection(Obstacle* obstacle) {
    if (obstacle->getDirection() == Direction::North || obstacle->getDirection() == Direction::South) {
        obstacle->setRotation(90);
    }
}

Obstacle* Obstacle::create(float topLength, float gapLength, float bottomLength, Direction direction) {
    const auto obstacleSize = Size{Column::defaultLength, topLength + gapLength + bottomLength};
    const auto obstacle = Obstacle::create(obstacleSize, direction);

    const auto bottomSize = Size{obstacleSize.width, bottomLength};
    const auto topSize = Size{obstacleSize.width, topLength};
    const auto bottomPosition = Vec2{bottomSize.width / 2, bottomSize.height / 2};
    const auto topPosition = Vec2{topSize.width / 2, bottomLength + gapLength + topSize.height / 2};

    setFrameForTextureSprite(obstacle->bottom, bottomPosition, bottomSize);
    setFrameForTextureSprite(obstacle->top, topPosition, topSize);
    setRotationForDirection(obstacle);
    return obstacle;
}

void Obstacle::positionInWorld(Rect world) {
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

Vec2 worldPosition(const Node& node) {
    return node.getParent()->convertToWorldSpace(node.getBoundingBox().origin);
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
