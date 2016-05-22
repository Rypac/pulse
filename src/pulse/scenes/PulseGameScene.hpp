#pragma once

#include "cocos2d.h"
#include "pulse/scenes/GameScene.hpp"
#include "pulse/sprites/Obstacle.hpp"
#include "pulse/sprites/WrappedSprite.hpp"
#include "pulse/models/GameState.hpp"

namespace pulse {

class PulseGameScene : public GameScene {
  public:
    static PulseGameScene* create(GameOptions& options);

    void update(float dt) override;

    bool onContactBegan(cocos2d::PhysicsContact& contact);
    bool onContactPreSolve(cocos2d::PhysicsContact& contact, cocos2d::PhysicsContactPreSolve& solve);
    void onContactSeparate(cocos2d::PhysicsContact& contact);
    bool onScreenCollision(const cocos2d::PhysicsContact& contact) const;

    using SceneCallback = std::function<void(PulseGameScene* scene)>;
    SceneCallback onEnterMenu;

  protected:
    PulseGameScene(GameOptions& options): options{options}, gameState{GameState{options}} {}
    virtual ~PulseGameScene();

    bool init() override;

    void runScene();
    void stopScene();
    void clearScene();
    void resetScene();

  private:
    void addBackground();
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

    GameOptions& options;
    WrappedSprite* player;
    cocos2d::Label* scoreLabel;
    std::list<Obstacle*> obstacles;
    GameState gameState;

    cocos2d::EventListenerTouchOneByOne* resetListener;
    cocos2d::EventListenerTouchOneByOne* timeScaleListener;
    cocos2d::EventListenerTouchOneByOne* playerTouchListener;
};

}
