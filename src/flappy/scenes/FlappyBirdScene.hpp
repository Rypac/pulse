#pragma once

#include "cocos2d.h"
#include <flappy/scenes/GameScene.hpp>
#include <flappy/sprites/FlappyBird.hpp>

namespace flappy {

class FlappyBirdScene : public GameScene {
public:
    CREATE_FUNC(FlappyBirdScene);

    bool init() override;
    void update(float) override;

    bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event) override;

private:
    void addFlappy();
    void addColumn();
    void generateColumns();
    void addTouchListeners();

    FlappyBird* flappy;
};

}
