#include "flappy/scenes/DeveloperMenuScene.hpp"
#include "flappy/ui/MenuSlider.hpp"
#include "flappy/utilities/Geometry.hpp"

using namespace cocos2d;
using namespace flappy;
using namespace flappy::ui;

Scene* DeveloperMenuScene::createScene(GameOptions& options, SceneCallback onSceneDismissed) {
    const auto scene = Scene::create();
    const auto layer = DeveloperMenuScene::create(options);
    layer->onSceneDismissed = onSceneDismissed;
    scene->addChild(layer);
    return scene;
}

DeveloperMenuScene* DeveloperMenuScene::create(GameOptions& options) {
    DeveloperMenuScene *scene = new (std::nothrow) DeveloperMenuScene();
    if (scene && scene->init(options)) {
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
    const auto preferences = SliderPreferences{0.1f, 2.0f, 1.0f, 0.1f};
    const auto obstacleFrequency = MenuSlider::create("Obstacle Frequency", preferences);
    obstacleFrequency->onValueChanged = [&](MenuSlider* slider) {
        options.obstacleFrequency = slider->value();
    };
    return obstacleFrequency;
}

MenuSlider* obstacleSpeed(GameOptions& options) {
    const auto prefs = SliderPreferences{1.0f, 5.0f, 3.5f, 0.1f};
    const auto obstacleSpeed = MenuSlider::create("Obstacle Travel Duration", prefs);
    obstacleSpeed->onValueChanged = [&](MenuSlider* slider) {
        options.obstacleSpeed = slider->value();
    };
    return obstacleSpeed;
}

MenuSlider* obstacleSlowMotionScale(GameOptions& options) {
    const auto prefs = SliderPreferences{0.1f, 1.0f, 0.3f, 0.1f};
    const auto obstacleSlowMotionScale = MenuSlider::create("Obstacle Slow Motion Speed", prefs);
    obstacleSlowMotionScale->onValueChanged = [&](MenuSlider* slider) {
        options.slowMotionTimeScale.environment = slider->value();
    };
    return obstacleSlowMotionScale;
}

MenuSlider* playerSlowMotionScale(GameOptions& options) {
    const auto prefs = SliderPreferences{0.1f, 1.0f, 0.3f, 0.1f};
    const auto playerSlowMotionScale = MenuSlider::create("Player Slow Motion Speed", prefs);
    playerSlowMotionScale->onValueChanged = [&](MenuSlider* slider) {
        options.slowMotionTimeScale.player = slider->value();
    };
    return playerSlowMotionScale;
}

bool DeveloperMenuScene::init(GameOptions& options) {
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

    return true;
}

void DeveloperMenuScene::addSliders() {
    const auto origin = geometry::topLeftOf(frame);
    const auto horizontalInset = frame.size.width * 0.1f;
    const auto verticalInset = 80;
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
