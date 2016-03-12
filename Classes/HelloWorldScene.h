#pragma once

#include "cocos2d.h"

class HelloWorld : public cocos2d::Layer {
public:
    static cocos2d::Scene* createScene();

    virtual bool init();

    void startColumnGenerator(cocos2d::Rect frame);

    CREATE_FUNC(HelloWorld);
};
