#pragma once

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include <string>

namespace pulse {
namespace ui {

class MenuSlider : public cocos2d::Node {
  public:
    struct Preferences {
        float minimum;
        float maximum;
        float initial;
        float increment;

        float range() const {
            return maximum - minimum;
        }
    };

    MenuSlider(const std::string& title, Preferences preferences);

    virtual void setContentSize(const cocos2d::Size& size) override;

    float value() const;
    void reset();

    using SliderCallback = std::function<void(MenuSlider* scene)>;
    SliderCallback onValueChanged;

  private:
    void setValue(float value);

    void addTitle(const std::string& title);
    void addSlider();
    void addCurrentValue();
    void updateDisplayedValue();

    cocos2d::Label* title;
    cocos2d::Label* currentValue;
    cocos2d::ui::Slider* slider;
    Preferences preferences;
};

} }
