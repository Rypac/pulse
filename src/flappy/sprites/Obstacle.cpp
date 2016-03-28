#include "flappy/sprites/Obstacle.hpp"
#include "flappy/sprites/SpriteTag.hpp"
#include "flappy/utilities/Geometry.hpp"

using namespace cocos2d;
using namespace flappy;

bool Obstacle::init() {
    if (!Sprite::init()) {
        return false;
    }

    top = Column::create();
    bottom = Column::create();
    gap = Sprite::create();
    addChild(top);
    addChild(bottom);
    addChild(gap);

    return true;
}

Obstacle* Obstacle::create(Size size, Direction direction) {
    const auto obstacle = Obstacle::create();
    obstacle->setPosition(0, 0);
    obstacle->setContentSize(size);
    obstacle->direction = direction;
    return obstacle;
}

static PhysicsBody* physicsBodyForSprite(SpriteTag tag, Size size) {
    const auto physicsBody = PhysicsBody::createBox(size);
    physicsBody->setDynamic(true);
    physicsBody->setGravityEnable(false);
    physicsBody->setCategoryBitmask(static_cast<int>(tag));
    physicsBody->setCollisionBitmask(static_cast<int>(SpriteTag::None));
    physicsBody->setContactTestBitmask(static_cast<int>(SpriteTag::Hero));
    return physicsBody;
}

static void setFrameForTextureSprite(Sprite* sprite, Vec2 position, Size size) {
    sprite->setPosition(position);
    sprite->setContentSize(size);
    sprite->setTextureRect(sprite->getBoundingBox());
    sprite->setPhysicsBody(physicsBodyForSprite(SpriteTag::Obstacle, size));
}

static void setFrameForGap(Node* node, Vec2 position, Size size) {
    node->setPosition(position);
    node->setContentSize(size);
    node->setVisible(false);
    node->setPhysicsBody(physicsBodyForSprite(SpriteTag::Path, size));
}

static void setRotationForDirection(Obstacle* obstacle) {
    if (obstacle->getDirection() == Direction::North || obstacle->getDirection() == Direction::South) {
        obstacle->setRotation(90);
    }
}

float gapOffset(Size obstacle, Size gap, Direction direction) {
    switch (direction) {
        case Direction::North:
        case Direction::West: return obstacle.width + gap.width;
        case Direction::South:
        case Direction::East: return -gap.width;
    }
}

Obstacle* Obstacle::create(float topLength, float gapLength, float bottomLength, Direction direction) {
    const auto obstacleSize = Size{Column::defaultLength, topLength + gapLength + bottomLength};
    const auto obstacle = Obstacle::create(obstacleSize, direction);

    const auto bottomSize = Size{obstacleSize.width, bottomLength};
    const auto topSize = Size{obstacleSize.width, topLength};
    const auto gapSize = Size{2, gapLength};
    const auto bottomPosition = Vec2{obstacleSize.width / 2, bottomLength / 2};
    const auto gapPosition = Vec2{gapOffset(obstacleSize, gapSize, direction), bottomLength + gapLength / 2};
    const auto topPosition = Vec2{obstacleSize.width / 2, bottomLength + gapLength + topSize.height / 2};

    setFrameForTextureSprite(obstacle->bottom, bottomPosition, bottomSize);
    setFrameForTextureSprite(obstacle->top, topPosition, topSize);
    setFrameForGap(obstacle->gap, gapPosition, gapSize);
    setRotationForDirection(obstacle);
    return obstacle;
}

void Obstacle::positionInWorld(Rect world) {
    const auto body = getBoundingBox();
    setPosition(geometry::origin(body, world, direction));
    destination = geometry::destination(body, world, direction);
}

void Obstacle::runActions(ObstacleCallback onCompletion) {
    const auto moveToEdge = MoveTo::create(3, destination);
    const auto removeFromScene = RemoveSelf::create(true);
    const auto actionsCompleted = CallFunc::create([=]() {
        if (onCompletion) {
            onCompletion(this);
        }
    });
    const auto actions = Sequence::create(moveToEdge, removeFromScene, actionsCompleted, nullptr);
    runAction(actions);
}
