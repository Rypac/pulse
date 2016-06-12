#include "pulse/scenes/TitleScene.hpp"
#include "pulse/actions/AnimatedBackground.hpp"
#include "pulse/extensions/Ref.hpp"
#include "pulse/ui/Button.hpp"
#include "pulse/ui/Resources.hpp"
#include "pulse/utilities/Geometry.hpp"
#include "pulse/utilities/Callback.hpp"

using namespace pulse;
using namespace cocos2d;

TitleScene::TitleScene() {
    setBackground(ParticleSystemQuad::create(Resources::Particles::AmbientBackground));
    addPlayButton();
    addModesButton();
    addAchievmentsButton();
    addSettingsButton();
}

void TitleScene::addPlayButton() {
    const auto playButton = ui::Button::create(Resources::Buttons::Play);
    playButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    playButton->setPosition(Vec2{sceneFrame().getMaxX(), sceneFrame().getMinY() + 120});
    playButton->onTouchEnded = [this](auto ref) {
        safe_callback(onPlaySelected, this);
    };
    addChild(playButton);

    const auto particles = ParticleSystemQuad::create(Resources::Particles::ButtonBackground);
    playButton->runAction(autoreleased<AnimatedBackground>(particles));
}

void TitleScene::addModesButton() {
    const auto modesButton = ui::Button::create(Resources::Buttons::Modes);
    modesButton->setPosition(Vec2{sceneFrame().getMinX() + 100, sceneFrame().getMinY() + 120});
    modesButton->onTouchEnded = [this](auto ref) {
        safe_callback(onModesSelected, this);
    };
    addChild(modesButton);
}

void TitleScene::addAchievmentsButton() {
    const auto achievmentsButton = ui::Button::create(Resources::Buttons::Achievements);
    achievmentsButton->setPosition(Vec2{sceneFrame().getMinX() + 250, sceneFrame().getMinY() + 120});
    achievmentsButton->onTouchEnded = [this](auto ref) {
        safe_callback(onAchievementsSelected, this);
    };
    addChild(achievmentsButton);
}

void TitleScene::addSettingsButton() {
    const auto settingsButton = ui::Button::create(Resources::Buttons::Settings);
    settingsButton->setPosition(Vec2{sceneFrame().getMinX() + 400, sceneFrame().getMinY() + 120});
    settingsButton->onTouchEnded = [this](auto ref) {
        safe_callback(onSettingsSelected, this);
    };
    addChild(settingsButton);
}
