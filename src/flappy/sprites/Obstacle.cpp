#include "Obstacle.hpp"

using namespace flappy;

void Obstacle::runActions(ObstacleCallback onCompletion) const {
    auto actionsCompleted = [&]() {
        if (top->actionCompleted() && bottom->actionCompleted()) {
            if (onCompletion) {
                onCompletion(*this);
            }
        }
    };
    top->onActionsCompleted = [=](const auto& column) { actionsCompleted(); };
    bottom->onActionsCompleted = [=](const auto& column) { actionsCompleted(); };

    const auto bottomActions = bottom->actionSequence();
    const auto topActions = top->actionSequence();
    bottom->runAction(bottomActions);
    top->runAction(topActions);
}

bool Obstacle::collidesWith(cocos2d::Rect frame) {
    return frame.intersectsRect(top->getBoundingBox())
        || frame.intersectsRect(bottom->getBoundingBox());
}

bool Obstacle::passedBy(cocos2d::Rect frame) {
    return frame.getMinX() > top->getBoundingBox().getMaxX()
        && frame.getMinX() > bottom->getBoundingBox().getMaxX();
}
