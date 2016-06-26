#pragma once

#include "cocos2d.h"

namespace pulse {

struct NodeAnimator {
    cocos2d::Node* node;
    cocos2d::Action* entryAnimation;
    cocos2d::Action* exitAnimation;

    void runEntryAnimation() {
        if (node and entryAnimation) {
            node->runAction(entryAnimation);
        }
    }

    void runExitAnimation() {
        if (node and exitAnimation) {
            node->runAction(exitAnimation);
        }
    }
};

}
