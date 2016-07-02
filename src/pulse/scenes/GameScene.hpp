#pragma once

#include "pulse/scenes/PhysicsScene.hpp"
#include "pulse/sprites/Obstacle.hpp"
#include "pulse/sprites/Player.hpp"
#include "pulse/sprites/Score.hpp"
#include "pulse/models/GameState.hpp"

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
    void addPlayer();
    void addGameStateListeners();
    void updateListeners(bool isGameRunning);
    void scheduleObstacleGeneration();

    cocos2d::EventListener* timeScaleTouchListener();
    cocos2d::EventListener* playerTouchListener();
    cocos2d::EventListener* playerMovementListener();
    cocos2d::EventListener* collisionListener();

    Obstacle* generateObstacle();

    void checkForObstacleCollision(const cocos2d::PhysicsContact& contact);
    void handlePassedObstacle(Obstacle* obstacle);

    Player* player;
    Score* score;
    std::list<Obstacle*> obstacles;
    GameState gameState;
    std::vector<cocos2d::EventListener*> gameListeners;
};

}
