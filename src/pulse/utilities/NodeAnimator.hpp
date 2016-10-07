#pragma once

#include "cocos2d.h"

namespace pulse {

class NodeAnimator {
  public:
    NodeAnimator()
    : NodeAnimator{nullptr} {}

    NodeAnimator(cocos2d::Node* node)
    : node_{node}
    , entryAnimation_{nullptr}
    , exitAnimation_{nullptr} {
        if (node_) {
            node_->retain();
        }
    }

    ~NodeAnimator() {
        CC_SAFE_RELEASE(node_);
        CC_SAFE_RELEASE(entryAnimation_);
        CC_SAFE_RELEASE(exitAnimation_);
    }

    NodeAnimator& operator=(NodeAnimator&& other) {
        node_ = other.node_;
        entryAnimation_ = other.entryAnimation_;
        exitAnimation_ = other.exitAnimation_;
        CC_SAFE_RETAIN(node_);
        CC_SAFE_RETAIN(entryAnimation_);
        CC_SAFE_RETAIN(exitAnimation_);
        return *this;
    }

    void setEntryAnimation(cocos2d::Action* animation) {
        entryAnimation_ = animation;
        entryAnimation_->retain();
    }

    void setExitAnimation(cocos2d::Action* animation) {
        exitAnimation_ = animation;
        exitAnimation_->retain();
    }

    void runEntryAnimation() {
        if (node_ and entryAnimation_) {
            node_->runAction(entryAnimation_);
        }
    }

    void runExitAnimation() {
        if (node_ and exitAnimation_) {
            node_->runAction(exitAnimation_);
        }
    }

  private:
    cocos2d::Node* node_;
    cocos2d::Action* entryAnimation_;
    cocos2d::Action* exitAnimation_;
};

}  // pulse
