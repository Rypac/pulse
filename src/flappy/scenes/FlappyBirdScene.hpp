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
    void initScene() override;
    void clearScene() override;
    void update(float dt) override;

    bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event) override;
    void onAccelerationDetected(cocos2d::Acceleration* acc, cocos2d::Event* event);
    void onMenuPause(cocos2d::Ref* menuItem);

  private:
    void addMenuOptions();
    void addScoreLabel();
    void addFlappy();
    void addObstacle();
    void generateObstacles();
    void addTouchListeners();
    void addAccelerometerListeners();
    void updateScore();
    void updateIncomingObstacles();

    bool collisionDetected() const;
    bool collisionWithEnvironment() const;
    bool collisionWithObstacles(const std::list<Obstacle*>& obstacles) const;

    Obstacle* generateObstacle() const;
    std::optional<Obstacle*> closestIncomingObstacle() const;

    FlappyBird* flappy;
    cocos2d::Label* scoreLabel;
    std::list<Obstacle*> passedObstacles;
    std::list<Obstacle*> incomingObstacles;
    GameState gameState;
};

}
