#include "pulse/scenes/DeveloperMenuScene.hpp"
#include "pulse/2d/Geometry.hpp"
#include "pulse/extensions/Ref.hpp"
#include "pulse/ui/Button.hpp"
#include "pulse/ui/MenuSlider.hpp"
#include "pulse/ui/Resources.hpp"
#include "pulse/utilities/Callback.hpp"

using namespace cocos2d;
using namespace pulse;
using namespace pulse::ui;

MenuSlider* obstacleFrequency(GameOptions& options) {
    const auto preferences = MenuSlider::Preferences{0.1f, 2.0f, options.obstacleFrequency, 0.1f};
    const auto slider = autoreleased<MenuSlider>("Obstacle Frequency (s)", preferences);
    slider->onValueChanged = [&](MenuSlider* slider) {
        options.obstacleFrequency = slider->value();
    };
    return slider;
}

MenuSlider* obstacleFrequencyStep(GameOptions& options) {
    const auto preferences = MenuSlider::Preferences{0.0f, 1.0f, options.obstacleFrequencyStep, 0.05f};
    const auto slider = autoreleased<MenuSlider>("Obstacle Frequency Increase (s)", preferences);
    slider->onValueChanged = [&](MenuSlider* slider) {
        options.obstacleFrequencyStep = slider->value();
    };
    return slider;
}

MenuSlider* obstacleSpeed(GameOptions& options) {
    const auto preferences = MenuSlider::Preferences{1.0f, 15.0f, options.obstacleSpeed, 0.1f};
    const auto slider = autoreleased<MenuSlider>("Obstacle Travel Duration (s)", preferences);
    slider->onValueChanged = [&](MenuSlider* slider) {
        options.obstacleSpeed = slider->value();
    };
    return slider;
}

MenuSlider* obstacleSpeedStep(GameOptions& options) {
    const auto preferences = MenuSlider::Preferences{0.0f, 1.0f, options.obstacleSpeedStep, 0.05f};
    const auto slider = autoreleased<MenuSlider>("Obstacle Travel Duration Reduction (s)", preferences);
    slider->onValueChanged = [&](MenuSlider* slider) {
        options.obstacleSpeedStep = slider->value();
    };
    return slider;
}

MenuSlider* obstacleDefeatedThreshold(GameOptions& options) {
    const auto preferences = MenuSlider::Preferences{1.0f, 20.0f, static_cast<float>(options.obstacleDefeatedThreshold), 1.0f};
    const auto slider = autoreleased<MenuSlider>("Obstacles To Defeat", preferences);
    slider->onValueChanged = [&](MenuSlider* slider) {
        options.obstacleDefeatedThreshold = static_cast<int>(slider->value());
    };
    return slider;
}

MenuSlider* obstacleSlowMotionScale(GameOptions& options) {
    const auto preferences = MenuSlider::Preferences{0.0f, 1.0f, options.slowMotionTimeScale.environment, 0.1f};
    const auto slider = autoreleased<MenuSlider>("Obstacle Slow Motion Speed", preferences);
    slider->onValueChanged = [&](MenuSlider* slider) {
        options.slowMotionTimeScale.environment = slider->value();
    };
    return slider;
}

MenuSlider* playerSlowMotionScale(GameOptions& options) {
    const auto preferences = MenuSlider::Preferences{0.1f, 10.0f, options.slowMotionTimeScale.player, 0.1f};
    const auto slider = autoreleased<MenuSlider>("Player Slow Motion Speed", preferences);
    slider->onValueChanged = [&](MenuSlider* slider) {
        options.slowMotionTimeScale.player = slider->value();
    };
    return slider;
}

DeveloperMenuScene::DeveloperMenuScene(GameOptions& options) {
    setBackground(LayerColor::create(Color4B::BLACK));
    addSliders({
        obstacleFrequency(options),
        obstacleFrequencyStep(options),
        obstacleSpeed(options),
        obstacleSpeedStep(options),
        obstacleDefeatedThreshold(options),
        obstacleSlowMotionScale(options),
        playerSlowMotionScale(options)
    });
    addBackButton();
}

void DeveloperMenuScene::addSliders(std::vector<ui::MenuSlider*> sliders) {
    const auto origin = geometry::topLeftOf(sceneFrame());
    const auto horizontalInset = sceneFrame().size.width * 0.1f;
    const auto verticalInset = 80;
    const auto sliderSize = Size{sceneFrame().size.width - horizontalInset * 2, 80};
    auto sliderOrigin = Vec2{origin.x + horizontalInset, origin.y - verticalInset};

    for (const auto slider : sliders) {
        slider->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
        slider->setPosition(sliderOrigin);
        slider->setContentSize(sliderSize);
        addChild(slider);

        sliderOrigin.y -= sliderSize.height;
    }
}

void DeveloperMenuScene::addBackButton() {
    const auto backButton = ui::Button::create(Resources::Buttons::Home);
    backButton->setPosition(Vec2{sceneFrame().getMaxX() - 80, sceneFrame().getMinY() + 80});
    backButton->onTouchEnded = [this](auto ref) {
        safe_callback(onSceneDismissed, this);
    };
    addChild(backButton);
}
