#pragma once

#include "pulse/models/Accelerometer.hpp"
#include "pulse/models/GameOptions.hpp"

namespace pulse {

struct Powerup {
    float timer;
    float rate;
};

struct GameState {
    enum class TimeMode {
        Normal,
        SlowMotion,
    };

    GameState(const GameOptions& options);

    int currentScore() const;
    void incrementScore();

    void newGame();
    void gameOver();

    using Callback = std::function<void(void)>;
    using TimeModeCallback = std::function<void(TimeMode timeMode)>;
    Callback onNewGame;
    Callback onGameOver;
    Callback onScoreChanged;
    TimeModeCallback onTimeModeChanged;

    TimeMode currentMode() const;
    void enterMode(TimeMode mode);

    float playerTimeScale() const;
    float obstacleTimeScale() const;
    float environmentTimeScale() const;

    float obstacleSpeed() const;
    float obstacleFrequency() const;

    Powerup& powerup();

    Accelerometer& accelerometer();

    void reset();

  private:
    void setScore(int score);
    const TimeScale& timeScale() const;

    const GameOptions& options_;
    const TimeScale normalTimeScale_;
    int score_;
    TimeMode timeMode_;
    Powerup powerup_;
    Accelerometer accelerometer_;
};

}  // pulse
