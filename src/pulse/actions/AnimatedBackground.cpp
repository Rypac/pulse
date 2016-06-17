#include "pulse/actions/AnimatedBackground.hpp"
#include "pulse/extensions/Rect.hpp"

using pulse::AnimatedBackground;
using cocos2d::Action;

AnimatedBackground::AnimatedBackground(cocos2d::ParticleSystem* particles): particles_{particles} {
    particles_->retain();
}

AnimatedBackground::~AnimatedBackground() {
    if (particles_) {
        if (particles_->getParent() == getTarget()) {
            particles_->removeFromParent();
        }
        particles_->release();
    }
}

void AnimatedBackground::startWithTarget(cocos2d::Node* target) {
    Action::startWithTarget(target);

    auto targetFrame = target->getBoundingBox();
    targetFrame.origin = target->convertToNodeSpace(targetFrame.origin);
    particles_->setPosition(targetFrame.origin);
    particles_->setSourcePosition(rect::center(targetFrame));
    particles_->setPosVar(targetFrame.size / 2);
    target->addChild(particles_, -1);
}

void AnimatedBackground::stop() {
    Action::stop();
    particles_->removeFromParent();
}

bool AnimatedBackground::isDone() const {
    return !getTarget()->isRunning();
}
