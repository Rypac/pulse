#include "pulse/sprites/Score.hpp"
#include "pulse/extensions/Ref.hpp"
#include "pulse/ui/Resources.hpp"
#include <range/v3/algorithm/reverse.hpp>

using pulse::Score;
using namespace cocos2d;

Score* Score::create(int score) {
    const auto sprite = owned<Score>();
    if (sprite and sprite->initWithScore(score)) {
        sprite->autorelease();
        return sprite;
    }
    delete sprite;
    return nullptr;
}

bool Score::initWithScore(int score) {
    if (not Sprite::init()) {
        return false;
    }

    const auto& digitImages = Resources::Images::Score::Digits;
    do {
        auto digit = score % 10;
        score /= 10;
        digits_.emplace_back(Sprite::create(digitImages[digit]));
    } while (score != 0);

    ranges::reverse(digits_);

    for (auto&& digit : digits_) {
        addChild(digit);
    }

    const auto digitSize = digits_[0]->getContentSize();
    const auto combinedDigitSize = Size{digitSize.width * digits_.size(), digitSize.height};
    setContentSize(combinedDigitSize);

    return true;
}

void Score::setContentSize(const cocos2d::Size& size) {
    Sprite::setContentSize(size);

    const auto digitSize = Size{size.width / digits_.size(), size.height};
    auto position = Vec2{digitSize.width / 2, digitSize.height / 2};
    for (auto&& digit : digits_) {
        digit->setContentSize(digitSize);
        digit->setPosition(position);
        position.x += digitSize.width;
    }
}
