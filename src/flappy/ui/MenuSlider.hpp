#pragma once

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include <string>

namespace flappy {
namespace ui {

class MenuSlider : public cocos2d::Node {
  public:
    static MenuSlider* create(cocos2d::Rect frame, const std::string& title, float lowerBound = 0.0, float upperBound = 1.0, float defaultValue = 0.5);

    bool init(cocos2d::Rect frame, const std::string& title, float lowerBound, float upperBound, float defaultValue);

    float value() const {
        return currentSliderValue;
    }

  private:
    cocos2d::Label* title;
    cocos2d::Label* currentValue;
    cocos2d::ui::Slider* slider;
    float minimumSliderValue;
    float maximumSliderValue;
    float currentSliderValue;
};

} }
