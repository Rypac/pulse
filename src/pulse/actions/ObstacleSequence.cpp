#include "pulse/actions/ObstacleSequence.hpp"

using namespace pulse;
using namespace cocos2d;

ObstacleSequence* ObstacleSequence::create(Obstacle *obstacle, float indicatorDurection) {
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
    const auto delay = DelayTime::create(indicatorDuration);
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
    actions.pushBack(delay);
    actions.pushBack(removeIndicator);
    actions.pushBack(addObstacle);
    return Sequence::init(actions);
}
