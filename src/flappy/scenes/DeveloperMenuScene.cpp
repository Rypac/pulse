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

bool DeveloperMenuScene::init(GameOptions& options) {
    if (!GameScene::init()) {
        return false;
    }

    const auto prefs = SliderPreferences{0.0f, 1.0f, 0.2f, 0.1f};
    const auto accelerometerSensitivity = MenuSlider::create("Accelerometer Sensitivity", prefs);

    const auto obstacleFrequency = MenuSlider::create("Obstacle Frequency", prefs);
    obstacleFrequency->onValueChanged = [&](MenuSlider* slider) {
        options.obstacleFrequency = slider->value();
    };

    const auto obstacleSpeed = MenuSlider::create("Obstacle Speed", prefs);
    obstacleSpeed->onValueChanged = [&](MenuSlider* slider) {
        options.obstacleSpeed = slider->value();
    };

    const auto obstacleSlowMotionScale = MenuSlider::create("Obstacle Slow Motion Speed", prefs);
    obstacleSlowMotionScale->onValueChanged = [&](MenuSlider* slider) {
        options.slowMotionTimeScale.environment = slider->value();
    };

    const auto playerSlowMotionScale = MenuSlider::create("Player Slow Motion Speed", prefs);
    playerSlowMotionScale->onValueChanged = [&](MenuSlider* slider) {
        options.slowMotionTimeScale.player = slider->value();
    };

    sliders = std::vector<MenuSlider*>{
        accelerometerSensitivity,
        obstacleFrequency,
        obstacleSpeed,
        obstacleSlowMotionScale,
        playerSlowMotionScale
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
