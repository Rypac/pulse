#include "pulse/scenes/GameOverScene.hpp"
#include "pulse/2d/Geometry.hpp"
#include "pulse/actions/CallbackAfter.hpp"
#include "pulse/actions/SequenceBuilder.hpp"
#include "pulse/extensions/Ref.hpp"
#include "pulse/sprites/Score.hpp"
#include "pulse/ui/Button.hpp"
#include "pulse/ui/Resources.hpp"
#include "pulse/utilities/Callback.hpp"

using namespace pulse;
using namespace cocos2d;

GameOverScene::GameOverScene(int score) {
    addBanner(score);
    addRestartButton();
    addHomeButton();

    runAction(entryAnimation());
}

void GameOverScene::addBanner(int score) {
    const auto banner = Sprite::create(Resources::Images::Score::Banner);
    banner->setRotation(-30.0f);

    const auto center = banner->getContentSize() / 2;
    const auto position = Vec2{center.width - 80, center.height};
    const auto bannerScore = Score::create(score);
    bannerScore->setPosition(position);
    banner->addChild(bannerScore);

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
    restartButton->disableAfterClick(true);
    restartButton->setRotation(-30.0f);
    restartButton->onTouchEnded = [this](auto ref) {
        this->runAction(autoreleased<CallbackAfter>(this->exitAnimation(), [this]() {
            safe_callback(onRestartGame, this);
        }));
    };

    const auto destination = Vec2{sceneFrame().getMaxX() - 195, sceneFrame().getMinY() + 65};
    const auto start = geometry::entryPosition(Direction::East, sceneFrame(), destination, restartButton);
    restartButton->setPosition(start);
    addChild(restartButton);

    restartAnimator_ = NodeAnimator(restartButton);
    restartAnimator_.setEntryAnimation(MoveTo::create(0.2, destination));
    restartAnimator_.setExitAnimation(MoveTo::create(0.2, start));
}

void GameOverScene::addHomeButton() {
    const auto homeButton = ui::Button::create(Resources::Buttons::Home);
    homeButton->disableAfterClick(true);
    homeButton->setScale(0.0f);
    homeButton->setPosition(sceneFrame().getMinX() + 80, sceneFrame().getMaxY() - 80);
    homeButton->onTouchEnded = [this](auto ref) {
        this->runAction(autoreleased<CallbackAfter>(this->exitAnimation(), [this]() {
            safe_callback(onQuitGame, this);
        }));
    };
    addChild(homeButton);

    homeAnimator_ = NodeAnimator(homeButton);
    homeAnimator_.setEntryAnimation(ScaleTo::create(0.1, 1));
    homeAnimator_.setExitAnimation(ScaleTo::create(0.1, 0));
}

FiniteTimeAction* GameOverScene::entryAnimation() {
    return SequenceBuilder()
        .delay(0.05)
        .add([this]() { scoreAnimator_.runEntryAnimation(); })
        .delay(0.6)
        .add([this]() { restartAnimator_.runEntryAnimation(); })
        .delay(0.4)
        .add([this]() { homeAnimator_.runEntryAnimation(); })
        .delay(0.1)
        .build();
}

FiniteTimeAction* GameOverScene::exitAnimation() {
    return SequenceBuilder()
        .delay(0.1)
        .add([this]() { homeAnimator_.runExitAnimation(); })
        .delay(0.1)
        .add([this]() { restartAnimator_.runExitAnimation(); })
        .delay(0.4)
        .add([this]() { scoreAnimator_.runExitAnimation(); })
        .delay(0.4)
        .build();
}
