#include "pulse/sprites/Score.hpp"
#include <range/v3/algorithm/reverse.hpp>
#include "pulse/extensions/Ref.hpp"
#include "pulse/ui/Resources.hpp"

using pulse::Score;
using namespace cocos2d;

Score* Score::create(int score) {
    const auto sprite = owned<Score>();
    if (sprite and sprite->init()) {
        sprite->autorelease();
        sprite->setScore(score);
        return sprite;
    }
    delete sprite;
    return nullptr;
}

void Score::setScore(int score) {
    for (auto&& digit : digits_) {
        digit->removeFromParent();
    }
    digits_.clear();

    do {
        const auto digit = score % 10;
        score /= 10;
        digits_.emplace_back(Sprite::createWithSpriteFrameName(Resources::Images::Digits[digit]));
    } while (score != 0);

    ranges::reverse(digits_);

    for (auto&& digit : digits_) {
        addChild(digit);
    }

    const auto digitSize = digits_[0]->getContentSize();
    const auto combinedDigitSize = Size{digitSize.width * digits_.size(), digitSize.height};
    setContentSize(combinedDigitSize);
}

void Score::setContentSize(const cocos2d::Size& size) {
    Sprite::setContentSize(size);
    if (digits_.empty()) {
        return;
    }

    const auto digitSize = Size{size.width / digits_.size(), size.height};
    auto position = Vec2{digitSize.width / 2, digitSize.height / 2};
    for (auto&& digit : digits_) {
        digit->setContentSize(digitSize);
        digit->setPosition(position);
        position.x += digitSize.width;
    }
}
