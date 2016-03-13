#pragma once

#include <cpputils/optional.hpp>

#include "cocos2d.h"
#include "flappy/scenes/GameScene.hpp"
#include "flappy/sprites/FlappyBird.hpp"
#include "flappy/sprites/Obstacle.hpp"

namespace flappy {

class FlappyBirdScene : public GameScene {
public:
    CREATE_FUNC(FlappyBirdScene);

    bool init() override;
    void update(float) override;

    bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event) override;

    Obstacle* generateObstacle();

private:
    void addFlappy();
    void addObstacle();
    void generateObstacles();
    void addTouchListeners();
    void addKeyboardListeners();

    std::optional<const Obstacle*> nearestObstacle();

    FlappyBird* flappy;
    std::list<const Obstacle*> passedObstacles;
    std::list<const Obstacle*> incomingObstacles;
    int score;
};

}
