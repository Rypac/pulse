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

    void gameOver();
    bool isGameOver() const;

    TimeMode currentMode() const;
    void enterMode(TimeMode mode);

    float playerTimeScale() const;
    float environmentTimeScale() const;

    Accelerometer& accelerometer();

    void reset();

  private:
    const TimeScale& timeScale() const;

    bool gameIsOver_;
    const GameOptions& options_;
    const TimeScale normalTimeScale_;
    int score_;
    TimeMode timeMode_;
    Accelerometer accelerometer_;
};

}
