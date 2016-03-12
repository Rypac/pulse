#pragma once

#include "cocos2d.h"
#include <gsl/gsl.h>
#include <flappy/scenes/GameScene.hpp>

class FlappyBirdScene : public GameScene {
public:
    CREATE_FUNC(FlappyBirdScene);

    virtual bool init() override;

    void startColumnGenerator();
};
