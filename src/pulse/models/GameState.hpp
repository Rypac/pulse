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

    GameState(const GameOptions& options): options(options) {}

    std::function<void(TimeMode)> onTimeModeChanged;

    int currentScore() const {
        return score;
    }

    void incrementScore() {
        score++;
    }

    void gameOver() {
        gameIsOver = true;
    }

    bool isGameOver() const {
        return gameIsOver;
    }

    Accelerometer& accelerometer() {
        return accelerometer_;
    }

    TimeMode currentMode() const {
        return timeMode;
    }

    void enterMode(TimeMode mode) {
        timeMode = mode;
        if (onTimeModeChanged) {
            onTimeModeChanged(mode);
        }
    }

    float playerTimeScale() const {
        return timeScale().player;
    }

    float environmentTimeScale() const {
        return timeScale().environment;
    }

    void reset() {
        score = 0;
        timeMode = TimeMode::Normal;
        gameIsOver = false;
        accelerometer_.reset();
    }

  private:
    const TimeScale& timeScale() const {
        return timeMode == TimeMode::Normal ? normalTimeScale : options.slowMotionTimeScale;
    }

    bool gameIsOver{true};
    const GameOptions& options;
    const TimeScale normalTimeScale{1.0, 1.0};
    int score{0};
    TimeMode timeMode{TimeMode::Normal};
    Accelerometer accelerometer_{30};
};

}
