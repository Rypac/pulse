#pragma once

#include "cocos2d.h"
#include "pulse/movement/AccelerometerMovementSystem.hpp"
#include "pulse/scenes/PhysicsScene.hpp"
#include "pulse/sprites/Obstacle.hpp"
#include "pulse/sprites/WrappedSprite.hpp"
#include "pulse/models/GameState.hpp"

namespace pulse {

class PulseGameScene : public PhysicsScene {
  public:
    PulseGameScene(const GameOptions& options);
    virtual ~PulseGameScene();

    virtual void update(float dt) override;

    void startNewGame();

    bool onContactBegan(cocos2d::PhysicsContact& contact);
    bool onContactPreSolve(cocos2d::PhysicsContact& contact, cocos2d::PhysicsContactPreSolve& solve);
    void onContactSeparate(cocos2d::PhysicsContact& contact);

    using SceneCallback = std::function<void(PulseGameScene* scene)>;
    SceneCallback onEnterMenu;
    SceneCallback onSceneDismissed;

  private:
    void startScene();
    void stopScene();
    void resetScene();

    void addMenuOptions();
    void addScoreLabel();
    void addPlayer();
    void addResetGameTouchListener();
    void addGameStateListeners();
    void updateListeners(bool isGameRunning);
    void updateScore();
    void updateSceneTimeScale();
    void scheduleObstacleGeneration();

    cocos2d::EventListener* timeScaleTouchListener();
    cocos2d::EventListener* playerTouchListener();
    cocos2d::EventListener* playerMovementListener();
    cocos2d::EventListener* collisionListener();

    Obstacle* generateObstacle();

    void handlePassedObstacle(Obstacle* obstacle);

    WrappedSprite* player;
    cocos2d::Label* scoreLabel;
    std::list<Obstacle*> obstacles;
    GameState gameState;

    cocos2d::EventListenerTouchOneByOne* resetListener;
    std::vector<cocos2d::EventListener*> gameListeners;
};

}
