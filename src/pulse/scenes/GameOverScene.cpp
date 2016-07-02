#include "pulse/scenes/GameOverScene.hpp"
#include "pulse/2d/Geometry.hpp"
#include "pulse/actions/SequenceBuilder.hpp"
#include "pulse/extensions/Ref.hpp"
#include "pulse/ui/Button.hpp"
#include "pulse/ui/Resources.hpp"
#include "pulse/utilities/Callback.hpp"

using namespace pulse;
using namespace cocos2d;

GameOverScene::GameOverScene() {
    addBanner();
    addRestartButton();

    runAction(entryAnimation());
}

void GameOverScene::addBanner() {
    const auto banner = Sprite::create(Resources::Images::Score::Banner);
    banner->setRotation(-30.0f);

    const auto destination = Vec2{sceneFrame().getMidX() + 20, sceneFrame().getMidY() + 80};
    const auto start = geometry::entryPosition(Direction::West, sceneFrame(), destination, banner);
    banner->setPosition(start);
    addChild(banner);

    scoreAnimator_ = NodeAnimator(banner);
    scoreAnimator_.setEntryAnimation(MoveTo::create(0.25, destination));
    scoreAnimator_.setExitAnimation(MoveTo::create(0.25, start));
}

void GameOverScene::addRestartButton() {
    const auto restartButton = ui::Button::create(Resources::Buttons::Restart);
    restartButton->setRotation(-30.0f);
    restartButton->onTouchEnded = [this](auto ref) {
        this->runAction(this->exitAnimation());
    };

    const auto destination = Vec2{sceneFrame().getMaxX() - 195, sceneFrame().getMinY() + 65};
    const auto start = geometry::entryPosition(Direction::East, sceneFrame(), destination, restartButton);
    restartButton->setPosition(start);
    addChild(restartButton);

    restartAnimator_ = NodeAnimator(restartButton);
    restartAnimator_.setEntryAnimation(MoveTo::create(0.2, destination));
    restartAnimator_.setExitAnimation(MoveTo::create(0.2, start));
}

Action* GameOverScene::entryAnimation() {
    return SequenceBuilder()
        .delay(0.05)
        .add([this]() { scoreAnimator_.runEntryAnimation(); })
        .delay(0.6)
        .add([this]() { restartAnimator_.runEntryAnimation(); })
        .delay(0.4)
        .build();
}

Action* GameOverScene::exitAnimation() {
    return SequenceBuilder()
        .delay(0.05)
        .add([this]() { restartAnimator_.runExitAnimation(); })
        .delay(0.4)
        .add([this]() { scoreAnimator_.runExitAnimation(); })
        .delay(0.4)
        .add([this]() { safe_callback(onRestartGame, this); })
        .build();
}
