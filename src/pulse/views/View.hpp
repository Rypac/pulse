#pragma once

#include "cocos2d.h"

class View : public cocos2d::Scene {
  public:
    View();
    virtual ~View();

    virtual void setBackground(cocos2d::Node* background);
    virtual void removeBackground();

    bool residesInBounds(const cocos2d::Node& node) const;

    const cocos2d::Rect& frame() const {
        return frame_;
    }

    const cocos2d::Node* background() const {
        return background_;
    }

  private:
    cocos2d::Rect frame_;
    cocos2d::Node* background_;
};
