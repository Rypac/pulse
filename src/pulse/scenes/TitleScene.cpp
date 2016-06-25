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

void TitleScene::addTitle() {
    const auto title = Sprite::create(Resources::Images::Title::Logo);

    const auto angle = 30.0f;
    const auto& size = title->getContentSize();
    const auto destination = Vec2{sceneFrame().getMidX() + 20, sceneFrame().getMidY() + 80};
    const auto start = geometry::entryPosition(Direction::West, sceneFrame(), destination, size, angle);

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
    const auto& size = playButton->getContentSize();
    const auto destination = Vec2{sceneFrame().getMaxX() - 195, sceneFrame().getMinY() + 65};
    const auto start = geometry::entryPosition(Direction::East, sceneFrame(), destination, size, angle);

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
