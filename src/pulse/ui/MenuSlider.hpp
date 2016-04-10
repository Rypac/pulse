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
        float initiial;
        float increment;

        float range() const {
            return maximum - minimum;
        }
    };

    static MenuSlider* create(const std::string& title, Preferences preferences);

    bool init(const std::string& sliderTitle, Preferences preferences);

    virtual void setContentSize(const cocos2d::Size& size) override;

    float value() const;
    void reset();

    std::function<void(MenuSlider*)> onValueChanged;

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

    using cocos2d::Node::init;
};

} }
