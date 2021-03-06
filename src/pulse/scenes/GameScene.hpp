#pragma once

#include "pulse/models/GameState.hpp"
#include "pulse/scenes/PhysicsScene.hpp"
#include "pulse/sprites/Obstacle.hpp"
#include "pulse/sprites/Player.hpp"
#include "pulse/sprites/Score.hpp"

namespace pulse {

class GameScene : public PhysicsScene {
  public:
    GameScene(const GameOptions& options);
    virtual ~GameScene();

    void startNewGame();

    using SceneCallback = std::function<void(GameScene* scene)>;
    SceneCallback onEnterMenu;
    SceneCallback onSceneDismissed;

    using GameOverCallback = std::function<void(GameScene* scene, int score)>;
    GameOverCallback onGameOver;

  private:
    void startScene();
    void stopScene();
    void resetScene();

    void addMenuOptions();
    void addScoreLabel();
    void addPowerupMeter();
    void addPlayer();
    void addGameStateListeners();
    void updatePowerupMeter();
    void updateListeners(bool isGameRunning);
    void scheduleObstacleGeneration();

    cocos2d::EventListener* timeScaleTouchListener();
    cocos2d::EventListener* playerMovementListener();
    cocos2d::EventListener* collisionListener();

    Obstacle* generateObstacle();
    void startPowerupTimer();

    void playSlowdownAnimation();
    void playSpeedupAnimation();

    bool isObstacleCollision(const cocos2d::PhysicsContact& contact) const;
    bool isPathCollision(const cocos2d::PhysicsContact& contact) const;

    Player* player;
    Score* score;
    cocos2d::Sprite* powerupMeter;
    std::list<Obstacle*> obstacles;
    GameState gameState;
    std::vector<cocos2d::EventListener*> gameListeners;
};

}  // pulse
