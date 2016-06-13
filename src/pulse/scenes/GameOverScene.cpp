#include "pulse/scenes/GameOverScene.hpp"
#include "pulse/extensions/Ref.hpp"
#include "pulse/ui/Button.hpp"
#include "pulse/ui/Resources.hpp"
#include "pulse/utilities/Callback.hpp"

using namespace pulse;
using namespace cocos2d;

GameOverScene::GameOverScene() {
    addBanner();
    addRestartButton();
}

void GameOverScene::addBanner() {
    const auto title = Sprite::create(Resources::Images::Title::Logo);
    title->retain();
    title->setPosition(sceneFrame().getMidX() + 20, sceneFrame().getMidY() + 80);
    title->setRotation(-30.0f);
    addChild(title);
}

void GameOverScene::addRestartButton() {
    const auto restartButton = ui::Button::create(Resources::Buttons::Restart);
    restartButton->setPosition(Vec2{sceneFrame().getMaxX() - 195, sceneFrame().getMinY() + 65});
    restartButton->setRotation(-30.0f);
    restartButton->onTouchEnded = [this](auto ref) {
        safe_callback(onRestartGame, this);
    };
    addChild(restartButton);
}
