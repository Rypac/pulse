#pragma once

#include "cocos2d.h"

namespace pulse {
namespace ui {

class Button : public cocos2d::Sprite {
  public:
    Button();
    virtual ~Button();

    static Button* create(const std::string& filename);

    using TouchCallback = std::function<void(Button* self)>;
    TouchCallback onTouchBegan;
    TouchCallback onTouchEnded;
    TouchCallback onTouchCancelled;

    void disableAfterClick(bool disable) {
        disableAfterClick_ = disable;
    }

    void setTouchEnabled(bool enabled) {
        listener_->setEnabled(enabled);
    }

    void reactsToTouch(bool reacts) {
        reactsToTouch_ = reacts;
    }

    void setTouchScale(float scale) {
        touchScale_ = scale;
    }

    float touchScale() const {
        return touchScale_;
    }

  protected:
    virtual bool initWithTexture(cocos2d::Texture2D* texture, const cocos2d::Rect& rect, bool rotated) override;

    virtual bool onTouchDidBegin(cocos2d::Touch* touch, cocos2d::Event* event);
    virtual void onTouchDidMove(cocos2d::Touch* touch, cocos2d::Event* event);
    virtual void onTouchDidEnd(cocos2d::Touch* touch, cocos2d::Event* event);

  private:
    cocos2d::EventListenerTouchOneByOne* listener_;
    float touchScale_;
    float savedScale_;
    bool reactsToTouch_;
    bool cancelled_;
    bool disableAfterClick_;
};

} }
