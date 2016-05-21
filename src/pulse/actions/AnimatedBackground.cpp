#include "pulse/actions/AnimatedBackground.hpp"
#include "pulse/utilities/Geometry.hpp"

using pulse::AnimatedBackground;
using cocos2d::Action;

AnimatedBackground* AnimatedBackground::create(cocos2d::ParticleSystem* particles) {
    const auto action = new (std::nothrow) AnimatedBackground{particles};
    if (action) {
        particles->retain();
        action->autorelease();
    }
    return action;
}

AnimatedBackground::~AnimatedBackground() {
    if (particles) {
        if (particles->getParent() == getTarget()) {
            particles->removeFromParent();
        }
        particles->release();
    }
}

void AnimatedBackground::startWithTarget(cocos2d::Node* target) {
    Action::startWithTarget(target);

    auto targetFrame = target->getBoundingBox();
    targetFrame.origin = target->convertToNodeSpace(targetFrame.origin);
    particles->setPosition(targetFrame.origin);
    particles->setSourcePosition(geometry::centerOf(targetFrame));
    particles->setPosVar(targetFrame.size / 2);
    target->addChild(particles, -1);
}

void AnimatedBackground::stop() {
    Action::stop();
    particles->removeFromParent();
}

bool AnimatedBackground::isDone() const {
    return !getTarget()->isRunning();
}
