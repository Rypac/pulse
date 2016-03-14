#pragma once

#include <cpputils/optional.hpp>

#include "cocos2d.h"
#include "flappy/scenes/GameScene.hpp"
#include "flappy/sprites/FlappyBird.hpp"
#include "flappy/sprites/Obstacle.hpp"
#include "flappy/models/GameState.hpp"

namespace flappy {

class FlappyBirdScene : public GameScene {
public:
    CREATE_FUNC(FlappyBirdScene);

    bool init() override;
    void update(float) override;

    void initScene() override;
    void clearScene() override;

    bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event) override;
    void onMenuPause(cocos2d::Ref* menuItem);

    Obstacle* generateObstacle() const;

private:
    void addMenuOptions();
    void addScoreLabel();
    void addFlappy();
    void addObstacle();
    void generateObstacles();
    void addTouchListeners();
    void updateScore();

    std::optional<Obstacle*> nearestObstacle() const;

    FlappyBird* flappy;
    cocos2d::Label* scoreLabel;
    std::list<Obstacle*> passedObstacles{};
    std::list<Obstacle*> incomingObstacles{};
    GameState gameState{};
};

}
