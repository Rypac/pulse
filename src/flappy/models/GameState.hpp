#pragma once

#include "cocos2d.h"

namespace flappy {

struct GameState {

    struct TimeScale {

        TimeScale(): player_(1.0), environment_(1.0) {}

        float player() const {
            return player_;
        }

        void setPlayer(float scale) {
            player_ = scale;
        }

        float environment() const {
            return environment_;
        }

        void setEnvironment(float scale) {
            environment_ = scale;
        }

      private:
        float player_;
        float environment_;
    };

    GameState(): score(0), timeScale_(TimeScale{}), accelerometerReference(cocos2d::Vec3{1, 0, 1}) {}

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

    TimeScale& timeScale() {
        return timeScale_;
    }

    void reset() {
        score = 0;
        timeScale_.setPlayer(1.0);
        timeScale_.setEnvironment(1.0);
    }

  private:
    int score;
    TimeScale timeScale_;
    cocos2d::Vec3 accelerometerReference;
};

}
