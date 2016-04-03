#include "flappy/ui/MenuSlider.hpp"

#include <algorithm>
#include <iomanip>

using namespace cocos2d;
using namespace cocos2d::ui;
using namespace flappy::ui;

MenuSlider* MenuSlider::create(const std::string& title, SliderPreferences preferences) {
    MenuSlider *menuSlider = new (std::nothrow) MenuSlider();
    if (menuSlider && menuSlider->init(title, preferences)) {
        menuSlider->autorelease();
        return menuSlider;
    }
    CC_SAFE_DELETE(menuSlider);
    return nullptr;
}

bool MenuSlider::init(const std::string& sliderTitle, SliderPreferences sliderPreferences) {
    if (!Node::init()) {
        return false;
    }

    preferences = sliderPreferences;

    addTitle(sliderTitle);
    addSlider();
    addCurrentValue();
    reset();

    return true;
}

void MenuSlider::addTitle(const std::string& text) {
    title = Label::createWithTTF(text, "fonts/Arial.ttf", 24);
    title->setAnchorPoint(Vec2{0, 0.5});
    addChild(title);
}

void MenuSlider::addSlider() {
    slider = Slider::create();
    slider->loadBarTexture("slider_blank.png");
    slider->loadSlidBallTextures("slider_ball_normal.png", "slider_ball_pressed.png", "slider_ball_disabled.png");
    slider->loadProgressBarTexture("slider_filled.png");
    slider->setScale9Enabled(true);
    slider->setAnchorPoint(Vec2{0, 0.5});
    slider->addEventListener([this](auto ref, auto eventType) {
        if (eventType == Slider::EventType::ON_PERCENTAGE_CHANGED) {
            updateDisplayedValue();
            if (onValueChanged) {
                onValueChanged(this);
            }
        }
    });
    addChild(slider);
}

void MenuSlider::addCurrentValue() {
    currentValue = Label::createWithTTF("", "fonts/Arial.ttf", 24);
    currentValue->setAnchorPoint(Vec2{1.0, 0.5});
    addChild(currentValue);
}

void MenuSlider::setContentSize(const Size& frame) {
    Node::setContentSize(frame);

    const auto padding = frame.width * 0.05f;
    const auto titleWidth = frame.width * 0.35f;
    const auto valueWidth = frame.width * 0.05f;
    const auto sliderWidth = frame.width - titleWidth - valueWidth - padding * 2;

    title->setPosition(Vec2{0, 0});
    title->setContentSize(Size{titleWidth, frame.height});

    slider->setPosition(Vec2{titleWidth + padding, 0});
    slider->setContentSize(Size{sliderWidth, std::min(20.0f, frame.height)});

    currentValue->setPosition(Vec2{frame.width, 0});
    currentValue->setContentSize(Size{valueWidth, frame.height});
}

float MenuSlider::value() const {
    auto&& sliderValue = preferences.range() * (slider->getPercent() / 100.0f);
    return sliderValue + preferences.minimum;
}

void MenuSlider::setValue(float newValue) {
    auto&& offset = newValue - preferences.minimum;
    auto&& percentage = offset / preferences.range() * 100.0f;
    slider->setPercent(percentage);
    updateDisplayedValue();
}

void MenuSlider::reset() {
    setValue(preferences.initiial);
}

void MenuSlider::updateDisplayedValue() {
    std::stringstream stream;
    stream << std::fixed << std::setprecision(1) << value();
    currentValue->setString(stream.str());
}
