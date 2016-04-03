#include "flappy/scenes/DeveloperMenuScene.hpp"
#include "flappy/ui/MenuSlider.hpp"
#include "flappy/utilities/Geometry.hpp"

using namespace cocos2d;
using namespace flappy;
using namespace flappy::ui;

DeveloperMenuScene* DeveloperMenuScene::create(GameOptions& options) {
    DeveloperMenuScene *scene = new (std::nothrow) DeveloperMenuScene(options);
    if (scene && scene->init()) {
        scene->autorelease();
        return scene;
    }
    CC_SAFE_DELETE(scene);
    return nullptr;
}

MenuSlider* accelerometerSensitivity(GameOptions& options) {
    const auto prefs = SliderPreferences{0.0f, 1.0f, 0.2f, 0.1f};
    return MenuSlider::create("Accelerometer Sensitivity", prefs);
}

MenuSlider* obstacleFrequency(GameOptions& options) {
    const auto preferences = SliderPreferences{0.1f, 2.0f, options.obstacleFrequency, 0.1f};
    const auto obstacleFrequency = MenuSlider::create("Obstacle Frequency", preferences);
    obstacleFrequency->onValueChanged = [&](MenuSlider* slider) {
        options.obstacleFrequency = slider->value();
    };
    return obstacleFrequency;
}

MenuSlider* obstacleSpeed(GameOptions& options) {
    const auto preferences = SliderPreferences{1.0f, 15.0f, options.obstacleSpeed, 0.1f};
    const auto obstacleSpeed = MenuSlider::create("Obstacle Travel Duration", preferences);
    obstacleSpeed->onValueChanged = [&](MenuSlider* slider) {
        options.obstacleSpeed = slider->value();
    };
    return obstacleSpeed;
}

MenuSlider* obstacleSlowMotionScale(GameOptions& options) {
    const auto preferences = SliderPreferences{0.0f, 1.0f, options.slowMotionTimeScale.environment, 0.1f};
    const auto obstacleSlowMotionScale = MenuSlider::create("Obstacle Slow Motion Speed", preferences);
    obstacleSlowMotionScale->onValueChanged = [&](MenuSlider* slider) {
        options.slowMotionTimeScale.environment = slider->value();
    };
    return obstacleSlowMotionScale;
}

MenuSlider* playerSlowMotionScale(GameOptions& options) {
    const auto preferences = SliderPreferences{0.1f, 10.0f, options.slowMotionTimeScale.player, 0.1f};
    const auto playerSlowMotionScale = MenuSlider::create("Player Slow Motion Speed", preferences);
    playerSlowMotionScale->onValueChanged = [&](MenuSlider* slider) {
        options.slowMotionTimeScale.player = slider->value();
    };
    return playerSlowMotionScale;
}

bool DeveloperMenuScene::init() {
    if (!GameScene::init()) {
        return false;
    }

    sliders = std::vector<MenuSlider*>{
        accelerometerSensitivity(options),
        obstacleFrequency(options),
        obstacleSpeed(options),
        obstacleSlowMotionScale(options),
        playerSlowMotionScale(options)
    };

    addSliders();
    addExitButton();

    return true;
}

void DeveloperMenuScene::addSliders() {
    const auto origin = geometry::topLeftOf(frame);
    const auto horizontalInset = frame.size.width * 0.1f;
    const auto verticalInset = 100;
    const auto sliderSize = Size{frame.size.width - horizontalInset * 2, 80};
    auto sliderOrigin = Vec2{origin.x + horizontalInset, origin.y - verticalInset};

    for (const auto slider : sliders) {
        slider->setAnchorPoint(Vec2{0, 0});
        slider->setPosition(sliderOrigin);
        slider->setContentSize(sliderSize);
        addChild(slider);

        sliderOrigin.y -= sliderSize.height;
    }
}

void DeveloperMenuScene::addExitButton() {
    const auto exitImage = Sprite::create();
    exitImage->setColor(Color3B::RED);
    exitImage->setContentSize(Size{40, 40});
    exitImage->setTextureRect(exitImage->getBoundingBox());
    const auto exit = MenuItemSprite::create(exitImage, exitImage, [this](auto ref) {
        if (onSceneDismissed) {
            onSceneDismissed(this);
        }
    });
    exit->setAnchorPoint(Vec2{0, 1.0});
    exit->setPosition(Vec2{frame.origin.x + 20, frame.origin.y + frame.size.height - 20});
    const auto menu = Menu::create(exit, nullptr);
    menu->setPosition(Vec2::ZERO);
    addChild(menu, 1);
}
