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
    const auto height = size.height / 2.0f;
    const auto width = size.width / 2.0f;
    const auto x = width * std::cos(angle) + height * std::tan(angle);
    const auto y = width * std::sin(angle) + height * std::cos(angle);
    return Size{x, y};
}

Vec2 rotatedOffset(const Vec2& destination, float angle) {
    const auto x = destination.y / std::tan(angle);
    const auto y = destination.x * std::tan(angle);
    return Vec2{x, y};
}

Vec2 displacementOffset(const Size& size, float angle) {
    const auto x = size.height / 2.0f * std::sin(angle);
    const auto y = x * std::tan(angle);
    return Vec2{x, y};
}

Vec2 bottomLeftStartFor(const Vec2& origin, const Vec2& destination, const Size& size, float angle) {
    const auto sizeOffset = rotatedSize(size, angle);
    const auto offset = rotatedOffset(destination, angle);
    const auto diffY = destination.y - offset.y;
    const auto rotationOffset = displacementOffset(size, angle);
    return Vec2{origin.x - sizeOffset.width - rotationOffset.x, origin.y - sizeOffset.height + diffY + rotationOffset.y};
}

Vec2 topRightStartFor(const Vec2& origin, const Vec2& destination, const Size& size, float angle) {
    const auto sizeOffset = rotatedSize(size, angle);
    const auto offset = rotatedOffset(destination, angle);
    const auto diffY = destination.y - offset.y;
    const auto rotationOffset = displacementOffset(size, angle);
    return Vec2{origin.x + sizeOffset.width + rotationOffset.x, origin.y + sizeOffset.height + diffY - rotationOffset.y};
}

void TitleScene::addTitle() {
    const auto title = Sprite::create(Resources::Images::Title::Logo);

    const auto angle = 30.0f;
    const auto radians = MATH_DEG_TO_RAD(angle);
    const auto& size = title->getContentSize();
    const auto origin = Vec2{sceneFrame().getMinX(), sceneFrame().getMinY()};
    const auto destination = Vec2{sceneFrame().getMidX() + 20, sceneFrame().getMidY() + 80};
    const auto start = bottomLeftStartFor(origin, destination, size, radians);

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
    const auto origin = Vec2{sceneFrame().getMaxX(), sceneFrame().getMaxY()};
    const auto destination = Vec2{sceneFrame().getMaxX() - 195, sceneFrame().getMinY() + 65};
    const auto start = topRightStartFor(origin, destination, size, radians);

    playButton->setPosition(start);
    playButton->setRotation(-30.0f);
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
