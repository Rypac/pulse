#pragma once

#include "cocos2d.h"

namespace pulse {

class Score : public cocos2d::Sprite {
  public:
    static Score* create(int score);

    virtual void setContentSize(const cocos2d::Size& size) override;

  protected:
    bool initWithScore(int score);

  private:
    std::vector<cocos2d::Node*> digits_;
};

}
