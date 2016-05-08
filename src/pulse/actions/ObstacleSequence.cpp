#include "pulse/actions/ObstacleSequence.hpp"
#include "pulse/utilities/Vector.hpp"

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
    const auto spawnDelay = DelayTime::create(indicatorDuration);
    const auto removalDelay = DelayTime::create(0.25);
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
    std::vector<FiniteTimeAction*> actions{addIndicator, spawnDelay, addObstacle, removalDelay, removeIndicator};
    return Sequence::init(toVector(actions));
}
