#pragma once

#include "cocos2d.h"
#include "pulse/scenes/GameScene.hpp"
#include "pulse/sprites/Obstacle.hpp"
#include "pulse/sprites/WrappedSprite.hpp"
#include "pulse/models/GameState.hpp"

namespace pulse {

class PulseGameScene : public GameScene {
  public:
    PulseGameScene(GameOptions& options): options{options}, gameState{GameState{options}} {}

    static PulseGameScene* create(GameOptions& options);

    bool init() override;
    void setupScene();
    void clearScene();
    void resetScene();
    void runScene() override;
    void stopScene() override;
    void update(float dt) override;

    bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event) override;
    void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event) override;
    void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event) override;
    void on3dTouchDetected(cocos2d::Touch* touch);
    void onAccelerationDetected(cocos2d::Acceleration* acc, cocos2d::Event* event);
    bool onContactBegan(cocos2d::PhysicsContact& contact);
    bool onContactPreSolve(cocos2d::PhysicsContact& contact, cocos2d::PhysicsContactPreSolve& solve);
    void onContactSeparate(cocos2d::PhysicsContact& contact);
    bool onScreenCollision(const cocos2d::PhysicsContact& contact) const;

    using SceneCallback = std::function<void(PulseGameScene* scene)>;
    SceneCallback onEnterMenu;

  private:
    void addBackground();
    void addMenuOptions();
    void addScoreLabel();
    void addPlayer();
    void addObstacle();
    void addTouchListeners();
    void addAccelerometerListeners();
    void addCollisionListeners();
    void addGameStateListeners();
    void updateScore();
    void updateSceneTimeScale();
    void scheduleObstacleGeneration();
    void unscheduleObstacleGeneration();

    void handleGameOver();
    void handlePassedObstacle(Obstacle* obstacle);

    GameOptions& options;
    WrappedSprite* player;
    cocos2d::Sprite3D* background;
    cocos2d::Label* scoreLabel;
    cocos2d::Action* obstacleGenerator;
    std::list<Obstacle*> obstacles;
    GameState gameState;
};

}
