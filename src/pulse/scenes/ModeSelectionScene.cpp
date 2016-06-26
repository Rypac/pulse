#include "pulse/scenes/ModeSelectionScene.hpp"
#include "pulse/actions/CallbackAfter.hpp"
#include "pulse/extensions/Animate.hpp"
#include "pulse/extensions/Ref.hpp"
#include "pulse/ui/Button.hpp"
#include "pulse/ui/Font.hpp"
#include "pulse/ui/Resources.hpp"
#include "pulse/utilities/Callback.hpp"

using namespace cocos2d;
using namespace pulse;

ModeSelectionScene::Mode::Mode(GameMode mode, const std::string& name): mode{mode} {
    label = Label::createWithTTF(name, Font::System, 32);
    button = ui::Button::create(Resources::Buttons::Blank);
}

ModeSelectionScene::ModeSelectionScene(GameMode mode): mode_{mode} {
    setBackground(LayerColor::create(Color4B::BLACK));

    modes_ = {
        {GameMode::FreePlay, "Free Play"},
        {GameMode::Classic, "Classic"},
        {GameMode::Reverse, "Reverse"},
    };

    layoutModes();
    updateSelectedMode(selectedMode());
    addDismissListener();
}

void ModeSelectionScene::onEnterTransitionDidFinish() {
    animate::fadeIn(this->background());

    for (auto&& mode : modes_) {
        mode.button->onTouchEnded = [&](auto ref) { this->updateSelectedMode(mode.mode); };
        addChild(mode.label);
        addChild(mode.button);
        animate::scaleIn(mode.button);
        animate::scaleIn(mode.label);
    }
}

void ModeSelectionScene::updateSelectedMode(GameMode selectedMode) {
    mode_ = selectedMode;
    for (auto&& mode : modes_) {
        const auto& texture = mode.mode == selectedMode ? Resources::Buttons::Tick : Resources::Buttons::Blank;
        mode.button->setTexture(texture);
    }
}

void ModeSelectionScene::addDismissListener() {
    const auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->setSwallowTouches(true);
    touchListener->onTouchBegan = [this](auto touch, auto event) { return true; };
    touchListener->onTouchEnded = [this](auto touch, auto event) {
        for (auto&& mode : modes_) {
            animate::scaleOut(mode.button);
            animate::scaleOut(mode.label);
        }
        animate::fadeOut(this->background(), [this]() {
            safe_callback(onSceneDismissed, this);
        });
    };
    getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);
}

void ModeSelectionScene::layoutModes() {
    constexpr const auto verticalPadding = 50;
    constexpr const auto horizontalPadding = 175;
    const auto midX = sceneFrame().getMidX();
    const auto midY = sceneFrame().getMidY();
    modes_[0].button->setPosition(midX - horizontalPadding, midY + verticalPadding);
    modes_[0].label->setPosition(midX - horizontalPadding, midY - verticalPadding);

    modes_[1].button->setPosition(midX, midY + verticalPadding);
    modes_[1].label->setPosition(midX, midY - verticalPadding);

    modes_[2].button->setPosition(midX + horizontalPadding, midY + verticalPadding);
    modes_[2].label->setPosition(midX + horizontalPadding, midY - verticalPadding);
}
