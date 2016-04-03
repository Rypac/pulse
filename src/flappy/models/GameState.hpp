#pragma once

#include "cocos2d.h"
#include "flappy/models/GameOptions.hpp"

namespace flappy {

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

    cocos2d::Vec3 calibratedAccelerometerOffset() const {
        return accelerometerReference;
    }

    void calibrateAccelerometer(cocos2d::Vec3 offset) {
        accelerometerReference = offset;
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
    }

  private:
    const TimeScale& timeScale() const {
        return timeMode == TimeMode::Normal ? normalTimeScale : options.slowMotionTimeScale;
    }

    const GameOptions& options;
    const TimeScale normalTimeScale{1.0, 1.0};
    int score{0};
    TimeMode timeMode{TimeMode::Normal};
    cocos2d::Vec3 accelerometerReference{1, 0, 1};
};

}
