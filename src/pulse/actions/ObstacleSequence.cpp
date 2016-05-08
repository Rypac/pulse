#include "pulse/actions/ObstacleSequence.hpp"

using namespace pulse;
using namespace cocos2d;

ObstacleSequence* ObstacleSequence::create(Obstacle* obstacle, float indicatorDurection) {
    const auto sequence = new (std::nothrow) ObstacleSequence{obstacle, indicatorDurection};
    if (sequence && sequence->init()) {
        obstacle->retain();
        sequence->autorelease();
        return sequence;
    }
    delete sequence;
    return nullptr;
}

ObstacleSequence::~ObstacleSequence() {
    if (obstacle) {
        obstacle->release();
    }
    if (indicator) {
        if (!isDone()) {
            indicator->removeFromParent();
        }
        indicator->release();
    }
}

bool ObstacleSequence::init() {
    const auto obstacleSpawnDelay = DelayTime::create(indicatorDuration);
    const auto indicatorRemovalDelay = DelayTime::create(0.25);
    const auto addIndicator = CallFunc::create([this]() {
        indicator = ObstacleIndicator::create(obstacle->getDirection(), getTarget()->getBoundingBox());
        indicator->retain();
        getTarget()->addChild(indicator);
    });
    const auto removeIndicator = CallFunc::create([this]() {
        indicator->removeFromParent();
    });
    const auto addObstacle = CallFunc::create([this]() {
        getTarget()->addChild(obstacle);
    });
    auto actions = Vector<FiniteTimeAction*>();
    actions.pushBack(addIndicator);
    actions.pushBack(obstacleSpawnDelay);
    actions.pushBack(addObstacle);
    actions.pushBack(indicatorRemovalDelay);
    actions.pushBack(removeIndicator);
    return Sequence::init(actions);
}
