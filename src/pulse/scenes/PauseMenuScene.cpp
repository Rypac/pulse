#include "pulse/scenes/PauseMenuScene.hpp"
#include "pulse/ui/Button.hpp"
#include "pulse/ui/Resources.hpp"
#include "pulse/utilities/Callback.hpp"

using namespace cocos2d;
using namespace pulse;

PauseMenuScene::PauseMenuScene() {
    addResumeButton();
    addRestartButton();
    addQuitButton();
}

void PauseMenuScene::addResumeButton() {
    const auto resumeButton = ui::Button::create(Resources::Buttons::Resume);
    resumeButton->setPosition(sceneFrame().getMidX(), sceneFrame().getMidY());
    resumeButton->onTouchEnded = [this](auto ref) {
        safe_callback(onResumeGame, this);
    };
    addChild(resumeButton);
}

void PauseMenuScene::addRestartButton() {
    const auto restartButton = ui::Button::create(Resources::Buttons::Reset);
    restartButton->setPosition(sceneFrame().getMidX() + 175, sceneFrame().getMidY());
    restartButton->onTouchEnded = [this](auto ref) {
        safe_callback(onRestartGame, this);
    };
    addChild(restartButton);
}

void PauseMenuScene::addQuitButton() {
    const auto quitButton = ui::Button::create(Resources::Buttons::Home);
    quitButton->setPosition(sceneFrame().getMidX() - 175, sceneFrame().getMidY());
    quitButton->onTouchEnded = [this](auto ref) {
        safe_callback(onQuitGame, this);
    };
    addChild(quitButton);
}
