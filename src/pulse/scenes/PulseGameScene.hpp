#pragma once

#include "pulse/scenes/PhysicsScene.hpp"
#include "pulse/sprites/Obstacle.hpp"
#include "pulse/sprites/Player.hpp"
#include "pulse/models/GameState.hpp"

namespace pulse {

class PulseGameScene : public PhysicsScene {
  public:
    PulseGameScene(const GameOptions& options);
    virtual ~PulseGameScene();

    void startNewGame();

    using SceneCallback = std::function<void(PulseGameScene* scene)>;
    SceneCallback onEnterMenu;
    SceneCallback onSceneDismissed;
    SceneCallback onGameOver;

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
    cocos2d::Label* scoreLabel;
    std::list<Obstacle*> obstacles;
    GameState gameState;
    std::vector<cocos2d::EventListener*> gameListeners;
};

}
