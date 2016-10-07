#pragma mark

#include "pulse/actions/CallbackAfter.hpp"
#include "pulse/extensions/Ref.hpp"

namespace pulse {
namespace animate {

static const auto AnimationDuration = 0.15f;

inline void fadeIn(cocos2d::Node* node, const std::function<void()>& onCompletion = nullptr) {
    node->setCascadeOpacityEnabled(true);
    node->setOpacity(0);
    node->runAction(autoreleased<CallbackAfter>(cocos2d::FadeTo::create(AnimationDuration, 220), onCompletion));
}

inline void fadeOut(cocos2d::Node* node, const std::function<void()>& onCompletion = nullptr) {
    node->setCascadeOpacityEnabled(true);
    node->runAction(autoreleased<CallbackAfter>(cocos2d::FadeTo::create(AnimationDuration, 0), onCompletion));
}

inline void scaleIn(cocos2d::Node* node, const std::function<void()>& onCompletion = nullptr) {
    node->setScale(0);
    node->runAction(autoreleased<CallbackAfter>(cocos2d::ScaleTo::create(AnimationDuration, 1), onCompletion));
}

inline void scaleOut(cocos2d::Node* node, const std::function<void()>& onCompletion = nullptr) {
    node->runAction(autoreleased<CallbackAfter>(cocos2d::ScaleTo::create(AnimationDuration, 0), onCompletion));
}

}  // animate
}  // pulse
