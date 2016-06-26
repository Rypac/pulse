#pragma once

#include "cocos2d.h"
#include "pulse/extensions/Vector.hpp"

namespace pulse {

struct SequenceBuilder {

    SequenceBuilder& add(cocos2d::FiniteTimeAction* action) {
        actions.emplace_back(action);
        return *this;
    }

    SequenceBuilder& add(const std::function<void()>& action) {
        return add(cocos2d::CallFunc::create(action));
    }

    cocos2d::Action* build() const {
        return cocos2d::Sequence::create(toVector(actions));
    }

  private:
    std::vector<cocos2d::FiniteTimeAction*> actions;
};

}
