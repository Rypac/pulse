#pragma once

#include "pulse/views/View.hpp"

namespace pulse {

class SplashView : public View {
  public:
    SplashView();
    virtual ~SplashView();

    cocos2d::Sprite* image() const;
    cocos2d::Animation* animation() const;

  private:
    cocos2d::Sprite* image_;
};

}
