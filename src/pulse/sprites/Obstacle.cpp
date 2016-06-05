#include "pulse/sprites/Obstacle.hpp"

#include "pulse/actions/RemoveSelfWithCallback.hpp"
#include "pulse/extensions/Ref.hpp"
#include "pulse/sprites/ObstaclePhysicsBody.hpp"
#include "pulse/utilities/Geometry.hpp"

using namespace cocos2d;
using namespace pulse;

Obstacle* Obstacle::create() {
    const auto obstacle = new (std::nothrow) Obstacle();
    if (obstacle && obstacle->init()) {
        obstacle->autorelease();
        return obstacle;
    }
    delete obstacle;
    return nullptr;
}

bool Obstacle::init() {
    if (!Sprite::init()) {
        return false;
    }

    top = autoreleased<Column>();
    bottom = autoreleased<Column>();
    gap = Sprite::create();
    addChild(top);
    addChild(bottom);
    addChild(gap);

    return true;
}

void Obstacle::onEnter() {
    Sprite::onEnter();
    runActions();
}

Obstacle* Obstacle::create(Size size, Direction direction) {
    const auto obstacle = Obstacle::create();
    obstacle->setPosition(0, 0);
    obstacle->setContentSize(size);
    obstacle->direction = direction;
    return obstacle;
}

static void setFrameForTextureSprite(Sprite* sprite, Vec2 position, Size size) {
    sprite->setPosition(position);
    sprite->setContentSize(size);
    sprite->setTextureRect(sprite->getBoundingBox());
}

static void setFrameForGap(Node* node, Vec2 position, Size size) {
    node->setPosition(position);
    node->setContentSize(size);
    node->setVisible(false);
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

float durationForDirection(float duration, Direction direction) {
    switch (direction) {
        case Direction::North:
        case Direction::South: return duration;
        case Direction::East:
        case Direction::West: return duration * 1.5f;
    }
}

void Obstacle::runActions() {
    const auto actionsStarted = CallFunc::create([this]() {
        if (onStarted) {
            onStarted(this);
        }
    });
    const auto scaledDuration = durationForDirection(getSpeed(), direction);
    const auto moveToEdge = MoveTo::create(scaledDuration, destination);
    const auto removeFromScene = autoreleased<RemoveSelfWithCallback>([this]() {
        if (onCompletion) {
            onCompletion(this);
        }
    });
    runAction(Sequence::create(actionsStarted, moveToEdge, removeFromScene, nullptr));
}

void Obstacle::runDefeatedActions() {
    setCascadeOpacityEnabled(true);
    const auto fadeOut = FadeOut::create(0.5);
    const auto removeFromScene = autoreleased<RemoveSelfWithCallback>([this]() {
        if (onCompletion) {
            onCompletion(this);
        }
    });
    runAction(Sequence::createWithTwoActions(fadeOut, removeFromScene));
}

void Obstacle::setPhysicsBody(ObstaclePhysicsBody* body) {
    Sprite::setPhysicsBody(body);
}

ObstaclePhysicsBody* Obstacle::getPhysicsBody() const {
    return dynamic_cast<ObstaclePhysicsBody*>(Sprite::getPhysicsBody());
}
