#pragma once

#include "cocos2d.h"
#include "flappy/scenes/GameScene.hpp"
#include "flappy/sprites/Obstacle.hpp"
#include "flappy/models/GameState.hpp"

namespace flappy {

class FlappyBirdScene : public GameScene {
  public:
    FlappyBirdScene(const GameOptions& options): options(options), gameState(GameState{options}) {}

    static cocos2d::Scene* createScene(const GameOptions& options);
    static FlappyBirdScene* create(const GameOptions& options);

    bool init() override;
    void initScene() override;
    void clearScene() override;
    void resetScene() override;
    void update(float dt) override;
    void onEnter() override;

    bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event) override;
    void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event) override;
    void onAccelerationDetected(cocos2d::Acceleration* acc, cocos2d::Event* event);
    bool onContactBegan(cocos2d::PhysicsContact& contact);
    void onContactEnded(cocos2d::PhysicsContact& contact);
    void onMenuPause(cocos2d::Ref* menuItem);

  private:
    void addMenuOptions();
    void addScoreLabel();
    void addFlappy();
    void addObstacle(float dt);
    void addTouchListeners();
    void addAccelerometerListeners();
    void addCollisionListeners();
    void updateScore();
    void updateSceneTimeScale();
    void scheduleObstacleGeneration();

    void handleObstacleCollision(Obstacle* obstacle);
    void handlePassedObstacle(Obstacle* obstacle);

    const GameOptions& options;
    cocos2d::Sprite* flappy;
    cocos2d::Label* scoreLabel;
    std::list<Obstacle*> obstacles;
    GameState gameState;
};

}
