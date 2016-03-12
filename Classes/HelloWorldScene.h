#pragma once

#include "cocos2d.h"
#include "GameScene.hpp"

class HelloWorld : public GameScene {
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    void startColumnGenerator();

    CREATE_FUNC(HelloWorld);
};
