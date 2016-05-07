#pragma once

#include "cocos2d.h"

namespace pulse {

class ObstacleIndicator : public cocos2d::Sprite {
  public:
    static ObstacleIndicator* create();

  protected:
    virtual bool init() override;
};

}
