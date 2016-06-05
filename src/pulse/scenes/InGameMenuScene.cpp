#include "pulse/scenes/InGameMenuScene.hpp"
#include "pulse/ui/Button.hpp"
#include "pulse/ui/Resources.hpp"

using namespace cocos2d;
using namespace pulse;

InGameMenuScene::InGameMenuScene() {
    addResumeButton();
    addRestartButton();
    addQuitButton();
}

void InGameMenuScene::addResumeButton() {
    const auto resumeButton = ui::Button::create(Resources::Buttons::Resume);
    resumeButton->setPosition(sceneFrame().getMidX(), sceneFrame().getMidY());
    resumeButton->onTouchEnded = [this](auto ref) {
        if (onResumeGame) {
            onResumeGame(this);
        }
    };
    addChild(resumeButton);
}

void InGameMenuScene::addRestartButton() {
    const auto restartButton = ui::Button::create(Resources::Buttons::Retry);
    restartButton->setPosition(sceneFrame().getMidX() + 175, sceneFrame().getMidY());
    restartButton->onTouchEnded = [this](auto ref) {
        if (onRestartGame) {
            onRestartGame(this);
        }
    };
    addChild(restartButton);
}

void InGameMenuScene::addQuitButton() {
    const auto quitButton = ui::Button::create(Resources::Buttons::Home);
    quitButton->setPosition(sceneFrame().getMidX() - 175, sceneFrame().getMidY());
    quitButton->onTouchEnded = [this](auto ref) {
        if (onQuitGame) {
            onQuitGame(this);
        }
    };
    addChild(quitButton);
}
