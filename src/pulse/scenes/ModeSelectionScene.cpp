#include "pulse/scenes/ModeSelectionScene.hpp"
#include "pulse/ui/Button.hpp"
#include "pulse/ui/Resources.hpp"
#include "pulse/utilities/Geometry.hpp"

#include "range/v3/algorithm/for_each.hpp"

using namespace cocos2d;
using namespace pulse;

ModeSelectionScene* ModeSelectionScene::create() {
    const auto scene = new (std::nothrow) ModeSelectionScene();
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

    modeButtons = {
        resumeButton(),
        restartButton(),
        quitButton()
    };
    ranges::for_each(modeButtons, [this](auto button) {
        this->addChild(button);
    });

    addDismissListener();

    return true;
}

void ModeSelectionScene::updateSelectedMode(ui::Button* selectedButton) {
    ranges::for_each(modeButtons, [&](auto button) {
        const auto texture = button == selectedButton ? Resources::Buttons::Tick : Resources::Buttons::Blank;
        button->setTexture(texture);
    });
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

pulse::ui::Button* ModeSelectionScene::resumeButton() {
    const auto resumeButton = ui::Button::create(Resources::Buttons::Blank);
    resumeButton->setPosition(geometry::centerOf(sceneFrame()));
    resumeButton->onTouchEnded = [this](auto ref) { this->updateSelectedMode(ref); };
    return resumeButton;
}

pulse::ui::Button* ModeSelectionScene::restartButton() {
    const auto restartButton = ui::Button::create(Resources::Buttons::Blank);
    restartButton->setPosition(sceneFrame().getMidX() + 175, sceneFrame().getMidY());
    restartButton->onTouchEnded = [this](auto ref) { this->updateSelectedMode(ref); };
    return restartButton;
}

pulse::ui::Button* ModeSelectionScene::quitButton() {
    const auto quitButton = ui::Button::create(Resources::Buttons::Blank);
    quitButton->setPosition(sceneFrame().getMidX() - 175, sceneFrame().getMidY());
    quitButton->onTouchEnded = [this](auto ref) { this->updateSelectedMode(ref); };
    return quitButton;
}
