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
    playAnimator_.setExitAnimation(MoveTo::create(0.4, end));
}

void TitleScene::addModesButton() {
    const auto modesButton = ui::Button::create(Resources::Buttons::Modes);
    modesButton->setScale(0.0f);
    modesButton->setPosition(Vec2{sceneFrame().getMaxX() - 400, sceneFrame().getMinY() + 135});
    modesButton->onTouchEnded = [this](auto ref) {
        safe_callback(onModesSelected, this);
    };
    addChild(modesButton);

    modeAnimator_ = NodeAnimator(modesButton);
    modeAnimator_.setEntryAnimation(ScaleTo::create(0.1, 1));
    modeAnimator_.setExitAnimation(ScaleTo::create(0.1, 0));
}

void TitleScene::addAchievmentsButton() {
    const auto achievementsButton = ui::Button::create(Resources::Buttons::Achievements);
    achievementsButton->setScale(0.0f);
    achievementsButton->setPosition(Vec2{sceneFrame().getMaxX() - 240, sceneFrame().getMinY() + 230});
    achievementsButton->onTouchEnded = [this](auto ref) {
        safe_callback(onAchievementsSelected, this);
    };
    addChild(achievementsButton);

    settingsAnimator_ = NodeAnimator(achievementsButton);
    settingsAnimator_.setEntryAnimation(ScaleTo::create(0.1, 1));
    settingsAnimator_.setExitAnimation(ScaleTo::create(0.1, 0));
}

void TitleScene::addSettingsButton() {
    const auto settingsButton = ui::Button::create(Resources::Buttons::Settings);
    settingsButton->setScale(0.0f);
    settingsButton->setPosition(Vec2{sceneFrame().getMaxX() - 80, sceneFrame().getMinY() + 320});
    settingsButton->onTouchEnded = [this](auto ref) {
        safe_callback(onSettingsSelected, this);
    };
    addChild(settingsButton);

    achievementsAnimator_ = NodeAnimator(settingsButton);
    achievementsAnimator_.setEntryAnimation(ScaleTo::create(0.1, 1));
    achievementsAnimator_.setExitAnimation(ScaleTo::create(0.1, 0));
}

void TitleScene::runEntryAnimation() {
    const auto animations = Sequence::create(
        DelayTime::create(0.05),
        CallFunc::create([this]() { titleAnimator_.runEntryAnimation(); }),
        DelayTime::create(0.5),
        CallFunc::create([this]() { playAnimator_.runEntryAnimation(); }),
        DelayTime::create(0.4),
        CallFunc::create([this]() { modeAnimator_.runEntryAnimation(); }),
        DelayTime::create(0.1),
        CallFunc::create([this]() { settingsAnimator_.runEntryAnimation(); }),
        DelayTime::create(0.1),
        CallFunc::create([this]() { achievementsAnimator_.runEntryAnimation(); }),
        nullptr
    );
    runAction(animations);
}

void TitleScene::runExitAnimation() {
    const auto animations = Sequence::create(
        DelayTime::create(0.05),
        CallFunc::create([this]() { titleAnimator_.runExitAnimation(); }),
        DelayTime::create(0.4),
        CallFunc::create([this]() { playAnimator_.runExitAnimation(); }),
        DelayTime::create(0.3),
        CallFunc::create([this]() { modeAnimator_.runExitAnimation(); }),
        DelayTime::create(0.1),
        CallFunc::create([this]() { settingsAnimator_.runExitAnimation(); }),
        DelayTime::create(0.1),
        CallFunc::create([this]() { achievementsAnimator_.runExitAnimation(); }),
        DelayTime::create(0.3),
        CallFunc::create([this]() { safe_callback(onPlaySelected, this); }),
        nullptr
    );
    runAction(animations);
}
