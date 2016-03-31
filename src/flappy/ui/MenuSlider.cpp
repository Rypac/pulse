#include "flappy/ui/MenuSlider.hpp"

using namespace cocos2d;
using namespace cocos2d::ui;
using namespace flappy::ui;

MenuSlider* MenuSlider::create(Rect frame, const std::string& title, float lowerBound, float upperBound, float defaultValue) {
    MenuSlider *menuSlider = new (std::nothrow) MenuSlider();
    if (menuSlider && menuSlider->init(frame, title, lowerBound, upperBound, defaultValue)) {
        menuSlider->autorelease();
        return menuSlider;
    }
    CC_SAFE_DELETE(menuSlider);
    return nullptr;
}

bool MenuSlider::init(cocos2d::Rect frame, const std::string& sliderTitle, float lowerBound, float upperBound, float defaultValue) {
    if (!Node::init()) {
        return false;
    }

    minimumSliderValue = lowerBound;
    maximumSliderValue = upperBound;
    currentSliderValue = defaultValue;

    const auto sliderWidth = frame.size.width * 0.5f;
    const auto titleWidth = frame.size.width * 0.4f;
    const auto valueWidth = frame.size.width * 0.1f;

    title = Label::create();
    title->setString(sliderTitle);
    title->setPosition(Vec2{frame.origin.x, frame.origin.y});
    title->setContentSize(Size{titleWidth, frame.size.height});

    slider = Slider::create();
    slider->setPosition(Vec2{frame.origin.x + titleWidth, frame.origin.y});
    slider->setContentSize(Size{sliderWidth, frame.size.height});
    slider->addEventListener([this](auto ref, auto eventType) {
        if (eventType == Slider::EventType::ON_PERCENTAGE_CHANGED) {
            currentSliderValue = slider->getPercent();
            currentValue->setString(std::to_string(currentSliderValue));
        }
    });

    currentValue = Label::create();
    currentValue->setString(std::to_string(currentSliderValue));
    currentValue->setPosition(Vec2{frame.origin.x + titleWidth + sliderWidth, frame.origin.y});
    currentValue->setContentSize(Size{valueWidth, frame.size.height});

    return true;
}
