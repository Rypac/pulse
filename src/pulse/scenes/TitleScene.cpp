#include "pulse/scenes/TitleScene.hpp"
#include "pulse/2d/Geometry.hpp"
#include "pulse/actions/AnimatedBackground.hpp"
#include "pulse/extensions/Ref.hpp"
#include "pulse/ui/Button.hpp"
#include "pulse/ui/Resources.hpp"
#include "pulse/utilities/Callback.hpp"

using namespace pulse;
using namespace cocos2d;

TitleScene::TitleScene() {
    addTitle();
    addPlayButton();
    addModesButton();
    addAchievmentsButton();
    addSettingsButton();
}

Size rotatedSize(const Size& size, float angle) {
    const auto width = size.width * std::cos(angle);
    const auto height = size.width * std::sin(angle);
    return Size{width, height};
}

Vec2 rotatedOffset(const Size& size, float angle) {
    const auto x = size.height / 2.0f * std::sin(angle);
    const auto y = x * std::tan(angle);
    return Vec2{x, y};
}

Vec2 leftEntryPosition(const Rect& frame, const Vec2& destination, const Size& size, float angle = 0.0f) {
    const auto sizeOffset = rotatedSize(size, angle);
    const auto offset = rotatedOffset(size, angle);
    const auto origin = Vec2{frame.getMinX() - offset.x, destination.y - (destination.x - frame.getMinX()) * std::tan(angle) - offset.y};

    return Vec2{origin.x - sizeOffset.width / 2.0f, origin.y - sizeOffset.height / 2.0f};
}

Vec2 rightEntryPosition(const Rect& frame, const Vec2& destination, const Size& size, float angle = 0.0f) {
    const auto sizeOffset = rotatedSize(size, angle);
    const auto offset = rotatedOffset(size, angle);
    const auto origin = Vec2{frame.getMaxX() + offset.x, destination.y + (frame.getMaxX() - destination.x) * std::tan(angle) + offset.y};

    return Vec2{origin.x + sizeOffset.width / 2.0f, origin.y + sizeOffset.height / 2.0f};
}

void TitleScene::addTitle() {
    const auto title = Sprite::create(Resources::Images::Title::Logo);

    const auto angle = 30.0f;
    const auto radians = MATH_DEG_TO_RAD(angle);
    const auto& size = title->getContentSize();
    const auto destination = Vec2{sceneFrame().getMidX() + 20, sceneFrame().getMidY() + 80};
    const auto start = leftEntryPosition(sceneFrame(), destination, size, radians);

    title->setPosition(start);
    title->setRotation(-angle);
    title->setonEnterTransitionDidFinishCallback([=]() {
        title->runAction(MoveTo::create(0.5, destination));
    });
    addChild(title);
}

void TitleScene::addPlayButton() {
    const auto playButton = ui::Button::create(Resources::Buttons::Play);

    const auto angle = 30.0f;
    const auto radians = MATH_DEG_TO_RAD(angle);
    const auto& size = playButton->getContentSize();
    const auto destination = Vec2{sceneFrame().getMaxX() - 195, sceneFrame().getMinY() + 65};
    const auto start = rightEntryPosition(sceneFrame(), destination, size, radians);

    playButton->setPosition(start);
    playButton->setRotation(-angle);
    playButton->setonEnterTransitionDidFinishCallback([=]() {
        playButton->runAction(MoveTo::create(0.5, destination));
    });
    playButton->onTouchEnded = [this](auto ref) {
        safe_callback(onPlaySelected, this);
    };
    addChild(playButton);
}

void TitleScene::addModesButton() {
    const auto modesButton = ui::Button::create(Resources::Buttons::Modes);
    modesButton->setPosition(Vec2{sceneFrame().getMaxX() - 400, sceneFrame().getMinY() + 135});
    modesButton->onTouchEnded = [this](auto ref) {
        safe_callback(onModesSelected, this);
    };
    addChild(modesButton);
}

void TitleScene::addAchievmentsButton() {
    const auto achievmentsButton = ui::Button::create(Resources::Buttons::Achievements);
    achievmentsButton->setPosition(Vec2{sceneFrame().getMaxX() - 240, sceneFrame().getMinY() + 230});
    achievmentsButton->onTouchEnded = [this](auto ref) {
        safe_callback(onAchievementsSelected, this);
    };
    addChild(achievmentsButton);
}

void TitleScene::addSettingsButton() {
    const auto settingsButton = ui::Button::create(Resources::Buttons::Settings);
    settingsButton->setPosition(Vec2{sceneFrame().getMaxX() - 80, sceneFrame().getMinY() + 320});
    settingsButton->onTouchEnded = [this](auto ref) {
        safe_callback(onSettingsSelected, this);
    };
    addChild(settingsButton);
}
