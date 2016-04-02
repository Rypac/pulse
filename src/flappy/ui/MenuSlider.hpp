#pragma once

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include <string>

namespace flappy {
namespace ui {

struct SliderPreferences {
    float minimum;
    float maximum;
    float initiial;
    float increment;

    float range() const {
        return maximum - minimum;
    }
};

class MenuSlider : public cocos2d::Node {
  public:
    static MenuSlider* create(const std::string& title, SliderPreferences preferences);

    bool init(const std::string& sliderTitle, SliderPreferences preferences);

    virtual void setContentSize(const cocos2d::Size& size) override;

    float value() const;
    void setValue(float value);

    void reset();

  private:
    void updateDisplayedValue();

    cocos2d::Label* title;
    cocos2d::Label* currentValue;
    cocos2d::ui::Slider* slider;
    SliderPreferences preferences;
};

} }
