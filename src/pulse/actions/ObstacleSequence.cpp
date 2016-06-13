#include "pulse/actions/ObstacleSequence.hpp"
#include "pulse/extensions/Vector.hpp"

using namespace pulse;
using namespace cocos2d;

ObstacleSequence::ObstacleSequence(Obstacle* obstacle, float indicatorDuration)
: obstacle_{obstacle}
, indicator_{nullptr} {
    obstacle_->retain();
    const auto spawnDelay = DelayTime::create(indicatorDuration);
    const auto removalDelay = DelayTime::create(0.25);
    const auto addIndicator = CallFunc::create([this]() {
        indicator_ = ObstacleIndicator::create(obstacle_->getDirection(), getTarget()->getBoundingBox());
        indicator_->retain();
        getTarget()->addChild(indicator_);
    });
    const auto removeIndicator = CallFunc::create([this]() {
        indicator_->removeFromParent();
    });
    const auto addObstacle = CallFunc::create([this]() {
        getTarget()->addChild(obstacle_, 2);
    });
    std::vector<FiniteTimeAction*> actions{addIndicator, spawnDelay, addObstacle, removalDelay, removeIndicator};
    init(toVector(std::move(actions)));
}

ObstacleSequence::~ObstacleSequence() {
    CC_SAFE_RELEASE(obstacle_);
    if (indicator_) {
        indicator_->removeFromParent();
        indicator_->release();
    }
}
