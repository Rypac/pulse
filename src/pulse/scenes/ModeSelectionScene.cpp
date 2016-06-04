#include "pulse/scenes/ModeSelectionScene.hpp"
#include "pulse/ui/Button.hpp"
#include "pulse/ui/Font.hpp"
#include "pulse/ui/Resources.hpp"

using namespace cocos2d;
using namespace pulse;

ModeSelectionScene::Mode::Mode(GameMode mode, const std::string& name): mode{mode} {
    label = Label::createWithTTF(name, Font::System, 32);
    button = ui::Button::create(Resources::Buttons::Blank);
}

ModeSelectionScene* ModeSelectionScene::create(GameMode mode) {
    const auto scene = new (std::nothrow) ModeSelectionScene{mode};
    if (scene && scene->init()) {
        scene->autorelease();
        return scene;
    }
    delete scene;
    return nullptr;
}

bool ModeSelectionScene::init() {
    if (!GameScene::init()) {
        return false;
    }

    modes_ = {
        {GameMode::FreePlay, "Free Play"},
        {GameMode::Classic, "Classic"},
        {GameMode::Reverse, "Reverse"},
    };
    for (const auto& mode : modes_) {
        mode.button->onTouchEnded = [&](auto ref) { this->updateSelectedMode(mode.mode); };
        addChild(mode.label);
        addChild(mode.button);
    }

    layoutModes();
    updateSelectedMode(selectedMode());
    addDismissListener();

    return true;
}

void ModeSelectionScene::updateSelectedMode(GameMode selectedMode) {
    mode_ = selectedMode;
    for (const auto& mode : modes_) {
        const auto& texture = mode.mode == selectedMode ? Resources::Buttons::Tick : Resources::Buttons::Blank;
        mode.button->setTexture(texture);
    }
}

void ModeSelectionScene::addDismissListener() {
    const auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->setSwallowTouches(true);
    touchListener->onTouchBegan = [this](auto touch, auto event) { return true; };
    touchListener->onTouchEnded = [this](auto touch, auto event) {
        if (onSceneDismissed) {
            onSceneDismissed(this);
        }
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
