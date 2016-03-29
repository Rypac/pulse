#pragma once

#include "cocos2d.h"

namespace flappy {

struct GameState {

    enum class TimeMode {
        Normal,
        SlowMotion
    };

    struct TimeScale {
        float player;
        float environment;
    };

    GameState():
        score(0),
        timeMode(TimeMode::Normal),
        normalTimeScale(TimeScale{1.0, 1.0}),
        slowMotionTimeScale(TimeScale{0.4, 0.3}),
        accelerometerReference(cocos2d::Vec3{1, 0, 1}) {}

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

    void enterMode(TimeMode mode) {
        timeMode = mode;
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
        return timeMode == TimeMode::Normal ? normalTimeScale : slowMotionTimeScale;
    }

    const TimeScale normalTimeScale;
    const TimeScale slowMotionTimeScale;
    int score;
    TimeMode timeMode;
    cocos2d::Vec3 accelerometerReference;
};

}
