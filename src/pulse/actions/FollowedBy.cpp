#include "pulse/actions/FollowedBy.hpp"

using pulse::FollowedBy;
using cocos2d::Action;
using cocos2d::Node;

FollowedBy* FollowedBy::create(Node *follower) {
    FollowedBy* action = new (std::nothrow) FollowedBy{follower};
    if (action) {
        follower->retain();
        action->autorelease();
    }
    return action;
}

FollowedBy::~FollowedBy() {
    if (follower) {
        if (follower->getParent()) {
            follower->removeFromParent();
        }
        follower->release();
    }
}

void FollowedBy::startWithTarget(cocos2d::Node* target) {
    Action::startWithTarget(target);
    target->addChild(follower);
}

void FollowedBy::stop() {
    Action::stop();
    follower->removeFromParent();
}

bool FollowedBy::isDone() const {
    return !getTarget()->isRunning();
}

void FollowedBy::step(float dt) {
    follower->setPosition(getTarget()->convertToNodeSpace(getTarget()->getPosition()));
}
