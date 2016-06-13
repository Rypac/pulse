#pragma once

#include "cocos2d.h"
#include "pulse/models/GameOptions.hpp"
#include "pulse/models/Accelerometer.hpp"

namespace pulse {

struct GameState {

    enum class TimeMode {
        Normal,
        SlowMotion
    };

    GameState(const GameOptions& options);

    std::function<void(TimeMode)> onTimeModeChanged;

    int currentScore() const;
    void incrementScore();

    bool hasGameStarted() const;
    void newGame();
    void gameOver();

    using Callback = std::function<void(void)>;
    Callback onNewGame;
    Callback onGameOver;

    TimeMode currentMode() const;
    void enterMode(TimeMode mode);

    float playerTimeScale() const;
    float environmentTimeScale() const;

    float obstacleSpeed() const;
    float obstacleFrequency() const;

    Accelerometer& accelerometer();

    void reset();

  private:
    const TimeScale& timeScale() const;

    const GameOptions& options_;
    const TimeScale normalTimeScale_;
    int score_;
    bool gameStarted_;
    TimeMode timeMode_;
    Accelerometer accelerometer_;
};

}
