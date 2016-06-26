#include "pulse/scenes/TitleScene.hpp"
#include "pulse/2d/Geometry.hpp"
#include "pulse/ui/Button.hpp"
#include "pulse/ui/Resources.hpp"
#include "pulse/utilities/Callback.hpp"

using namespace pulse;
using namespace cocos2d;

TitleScene::TitleScene(): titleAnimator_(), playAnimator_() {
    addTitle();
    addPlayButton();
    addModesButton();
    addAchievmentsButton();
    addSettingsButton();

    setonEnterTransitionDidFinishCallback([this]() {
        this->runEntryAnimation();
    });
}

void TitleScene::addTitle() {
    const auto title = Sprite::create(Resources::Images::Title::Logo);

    const auto angle = 30.0f;
    const auto& size = title->getContentSize();
    const auto destination = Vec2{sceneFrame().getMidX() + 20, sceneFrame().getMidY() + 80};
    const auto start = geometry::entryPosition(Direction::West, sceneFrame(), destination, size, angle);
    const auto end = geometry::entryPosition(Direction::East, sceneFrame(), destination, size, angle);

    title->setPosition(start);
    title->setRotation(-angle);
    addChild(title);

    titleAnimator_ = NodeAnimator(title);
    titleAnimator_.setEntryAnimation(MoveTo::create(0.25, destination));
    titleAnimator_.setExitAnimation(MoveTo::create(0.25, end));
}

void TitleScene::addPlayButton() {
    const auto playButton = ui::Button::create(Resources::Buttons::Play);

    const auto angle = 30.0f;
    const auto& size = playButton->getContentSize();
    const auto destination = Vec2{sceneFrame().getMaxX() - 195, sceneFrame().getMinY() + 65};
    const auto start = geometry::entryPosition(Direction::East, sceneFrame(), destination, size, angle);
    const auto end = geometry::entryPosition(Direction::West, sceneFrame(), destination, size, angle);

    playButton->setPosition(start);
    playButton->setRotation(-angle);
    playButton->onTouchEnded = [this](auto ref) {
        this->runExitAnimation();
    };
    addChild(playButton);

    playAnimator_ = NodeAnimator(playButton);
    playAnimator_.setEntryAnimation(MoveTo::create(0.2, destination));
    playAnimator_.setExitAnimation(MoveTo::create(0.2, end));
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

void TitleScene::runEntryAnimation() {
    const auto animations = Sequence::create(
        DelayTime::create(0.1),
        CallFunc::create([this]() { titleAnimator_.runEntryAnimation(); }),
        DelayTime::create(0.5),
        CallFunc::create([this]() { playAnimator_.runEntryAnimation(); }),
        nullptr
    );
    runAction(animations);
}

void TitleScene::runExitAnimation() {
    const auto animations = Sequence::create(
        DelayTime::create(0.1),
        CallFunc::create([this]() { titleAnimator_.runExitAnimation(); }),
        DelayTime::create(0.4),
        CallFunc::create([this]() { playAnimator_.runExitAnimation(); }),
        DelayTime::create(0.3),
        CallFunc::create([this]() { safe_callback(onPlaySelected, this); }),
        nullptr
    );
    runAction(animations);
}
