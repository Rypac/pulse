#include "pulse/ui/MenuSlider.hpp"
#include "pulse/ui/Font.hpp"
#include "pulse/ui/Resources.hpp"
#include "pulse/utilities/Callback.hpp"

#include <algorithm>
#include <iomanip>

using namespace cocos2d;
using namespace cocos2d::ui;
using namespace pulse::ui;

MenuSlider::MenuSlider(const std::string& title, const Preferences& preferences) {
    this->preferences = preferences;
    addTitle(title);
    addSlider();
    addCurrentValue();
    reset();
}

void MenuSlider::addTitle(const std::string& text) {
    title = Label::createWithTTF(text, Font::System, 28);
    title->setAnchorPoint(Vec2{0, 0.5});
    addChild(title);
}

void MenuSlider::addSlider() {
    slider = Slider::create();
    slider->loadBarTexture(Resources::Images::Menu::Slider::Empty);
    slider->loadSlidBallTextures(
        Resources::Images::Menu::Slider::Normal,
        Resources::Images::Menu::Slider::Selected,
        Resources::Images::Menu::Slider::Disabled
    );
    slider->loadProgressBarTexture(Resources::Images::Menu::Slider::Filled);
    slider->setScale9Enabled(true);
    slider->setAnchorPoint(Vec2{0, 0.5});
    slider->addEventListener([this](auto ref, auto eventType) {
        if (eventType == Slider::EventType::ON_PERCENTAGE_CHANGED) {
            this->updateDisplayedValue();
            safe_callback(onValueChanged, this);
        }
    });
    addChild(slider);
}

void MenuSlider::addCurrentValue() {
    currentValue = Label::createWithTTF("", Font::System, 28);
    currentValue->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    addChild(currentValue);
}

void MenuSlider::setContentSize(const Size& frame) {
    Node::setContentSize(frame);

    const auto padding = frame.width * 0.05f;
    const auto titleWidth = frame.width * 0.4f;
    const auto valueWidth = frame.width * 0.05f;
    const auto sliderWidth = frame.width - titleWidth - valueWidth - padding * 2;

    title->setPosition(Vec2::ZERO);
    title->setContentSize(Size{titleWidth, frame.height});

    slider->setPosition(Vec2{titleWidth + padding, 0});
    slider->setContentSize(Size{sliderWidth, std::min(20.0f, frame.height)});

    currentValue->setPosition(Vec2{frame.width, 0});
    currentValue->setContentSize(Size{valueWidth, frame.height});
}

float MenuSlider::value() const {
    const auto sliderValue = preferences.range() * (slider->getPercent() / 100.0f);
    return sliderValue + preferences.minimum;
}

void MenuSlider::setValue(float newValue) {
    const auto offset = newValue - preferences.minimum;
    const auto percentage = offset / preferences.range() * 100.0f;
    slider->setPercent(percentage);
    updateDisplayedValue();
}

void MenuSlider::reset() {
    setValue(preferences.initial);
}

void MenuSlider::updateDisplayedValue() {
    std::stringstream stream;
    stream << std::fixed << std::setprecision(1) << value();
    currentValue->setString(stream.str());
}
