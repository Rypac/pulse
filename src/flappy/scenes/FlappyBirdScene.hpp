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
    void reset() override;

    bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event) override;

    Obstacle* generateObstacle() const;

private:
    void addFlappy();
    void addObstacle();
    void generateObstacles();
    void addTouchListeners();
    void addKeyboardListeners();

    std::optional<Obstacle*> nearestObstacle() const;

    FlappyBird* flappy;
    std::list<Obstacle*> passedObstacles;
    std::list<Obstacle*> incomingObstacles;
    int score;
};

}
