#include "pulse/actions/FollowedBy.hpp"

using pulse::FollowedBy;
using namespace cocos2d;

FollowedBy::FollowedBy(Node* follower, const Vec2& offset): follower_{follower}, offset_(offset) {
    follower_->retain();
}

FollowedBy::~FollowedBy() {
    if (follower_) {
        if (follower_->getParent() == getTarget()) {
            follower_->removeFromParent();
        }
        follower_->release();
    }
}

void FollowedBy::startWithTarget(Node* target) {
    Action::startWithTarget(target);
    target->addChild(follower_);
}

void FollowedBy::stop() {
    Action::stop();
    follower_->removeFromParent();
}

bool FollowedBy::isDone() const {
    return not getTarget()->isRunning();
}

void FollowedBy::step(float dt) {
    const auto targetPosition = getTarget()->convertToNodeSpace(getTarget()->getPosition());
    follower_->setPosition(targetPosition + offset_);
}
