#include "pulse/actions/FollowedBy.hpp"

using pulse::FollowedBy;
using cocos2d::Action;
using cocos2d::Node;

FollowedBy::FollowedBy(Node* follower): follower_{follower} {
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
    return !getTarget()->isRunning();
}

void FollowedBy::step(float dt) {
    const auto targetPosition = getTarget()->convertToNodeSpace(getTarget()->getPosition());
    follower_->setPosition(targetPosition);
}
