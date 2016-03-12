#pragma once

#include "cocos2d.h"
#include <flappy/scenes/GameScene.hpp>
#include <flappy/sprites/FlappyBird.hpp>

namespace flappy {

class FlappyBirdScene : public GameScene {
public:
    CREATE_FUNC(FlappyBirdScene);

    virtual bool init() override;

private:
    void addFlappy();
    void addColumn();
    void generateColumns();

    FlappyBird* flappy;
};

}
