#include "pulse/scenes/DeveloperMenuScene.hpp"
#include "pulse/extensions/Ref.hpp"
#include "pulse/ui/Button.hpp"
#include "pulse/ui/MenuSlider.hpp"
#include "pulse/ui/Resources.hpp"
#include "pulse/utilities/Geometry.hpp"

using namespace cocos2d;
using namespace pulse;
using namespace pulse::ui;

MenuSlider* obstacleFrequency(GameOptions& options) {
    const auto preferences = MenuSlider::Preferences{0.1f, 2.0f, options.obstacleFrequency, 0.1f};
    const auto obstacleFrequency = autoreleased<MenuSlider>("Obstacle Frequency", preferences);
    obstacleFrequency->onValueChanged = [&](MenuSlider* slider) {
        options.obstacleFrequency = slider->value();
    };
    return obstacleFrequency;
}

MenuSlider* obstacleSpeed(GameOptions& options) {
    const auto preferences = MenuSlider::Preferences{1.0f, 15.0f, options.obstacleSpeed, 0.1f};
    const auto obstacleSpeed = autoreleased<MenuSlider>("Obstacle Travel Duration", preferences);
    obstacleSpeed->onValueChanged = [&](MenuSlider* slider) {
        options.obstacleSpeed = slider->value();
    };
    return obstacleSpeed;
}

MenuSlider* obstacleSlowMotionScale(GameOptions& options) {
    const auto preferences = MenuSlider::Preferences{0.0f, 1.0f, options.slowMotionTimeScale.environment, 0.1f};
    const auto obstacleSlowMotionScale = autoreleased<MenuSlider>("Obstacle Slow Motion Speed", preferences);
    obstacleSlowMotionScale->onValueChanged = [&](MenuSlider* slider) {
        options.slowMotionTimeScale.environment = slider->value();
    };
    return obstacleSlowMotionScale;
}

MenuSlider* playerSlowMotionScale(GameOptions& options) {
    const auto preferences = MenuSlider::Preferences{0.1f, 10.0f, options.slowMotionTimeScale.player, 0.1f};
    const auto playerSlowMotionScale = autoreleased<MenuSlider>("Player Slow Motion Speed", preferences);
    playerSlowMotionScale->onValueChanged = [&](MenuSlider* slider) {
        options.slowMotionTimeScale.player = slider->value();
    };
    return playerSlowMotionScale;
}

DeveloperMenuScene::DeveloperMenuScene(GameOptions& options): options(options) {
    setBackground(LayerColor::create(Color4B::BLACK));
    addSliders({
        obstacleFrequency(options),
        obstacleSpeed(options),
        obstacleSlowMotionScale(options),
        playerSlowMotionScale(options)
    });
    addBackButton();
}

void DeveloperMenuScene::addSliders(std::vector<ui::MenuSlider*> sliders) {
    const auto origin = geometry::topLeftOf(sceneFrame());
    const auto horizontalInset = sceneFrame().size.width * 0.1f;
    const auto verticalInset = 100;
    const auto sliderSize = Size{sceneFrame().size.width - horizontalInset * 2, 80};
    auto sliderOrigin = Vec2{origin.x + horizontalInset, origin.y - verticalInset};

    for (const auto slider : sliders) {
        slider->setAnchorPoint(Vec2::ZERO);
        slider->setPosition(sliderOrigin);
        slider->setContentSize(sliderSize);
        addChild(slider);

        sliderOrigin.y -= sliderSize.height;
    }
}

void DeveloperMenuScene::addBackButton() {
    const auto backButton = ui::Button::create(Resources::Buttons::Home);
    backButton->setPosition(Vec2{sceneFrame().getMaxX() - 100, sceneFrame().getMinY() + 100});
    backButton->onTouchEnded = [this](auto ref) {
        if (onSceneDismissed) {
            onSceneDismissed(this);
        }
    };
    addChild(backButton);
}
