#pragma once

#include "cocos2d.h"
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
    virtual void onEnterTransitionDidFinish() override;

    void resetScene();

    bool onContactBegan(cocos2d::PhysicsContact& contact);
    bool onContactPreSolve(cocos2d::PhysicsContact& contact, cocos2d::PhysicsContactPreSolve& solve);
    void onContactSeparate(cocos2d::PhysicsContact& contact);

    using SceneCallback = std::function<void(PulseGameScene* scene)>;
    SceneCallback onEnterMenu;
    SceneCallback onSceneDismissed;

  protected:
    void startScene();
    void stopScene();
    void clearScene();

  private:
    void addMenuOptions();
    void addScoreLabel();
    void addPlayer();
    void addResetGameTouchListener();
    void addTimeScaleTouchListener();
    void addPlayerTouchListener();
    void addPlayerMovementListener();
    void addCollisionListeners();
    void addGameStateListeners();
    void updateListeners(bool isGameRunning);
    void updateScore();
    void updateSceneTimeScale();
    void scheduleObstacleGeneration();

    Obstacle* generateObstacle();

    void handleGameOver();
    void handlePassedObstacle(Obstacle* obstacle);

    WrappedSprite* player;
    cocos2d::Label* scoreLabel;
    std::list<Obstacle*> obstacles;
    GameState gameState;

    cocos2d::EventListenerTouchOneByOne* resetListener;
    cocos2d::EventListenerTouchOneByOne* timeScaleListener;
    cocos2d::EventListenerTouchOneByOne* playerTouchListener;
};

}
