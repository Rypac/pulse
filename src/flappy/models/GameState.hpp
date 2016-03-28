#pragma once

#include "cocos2d.h"

namespace flappy {

struct GameState {

    GameState(): score(0), timeScale(1.0), accelerometerReference(cocos2d::Vec3{1, 0, 1}) {}

    int currentScore() const { return score; };
    void addToScore() { score++; };

    cocos2d::Vec3 calibratedAccelerometerOffset() const { return accelerometerReference; };
    void calibrateAccelerometer(cocos2d::Vec3 offset) { accelerometerReference = offset; };

    float getTimeScale() const { return timeScale; }
    void setTimeScale(float scale) { timeScale = scale; }

    void reset() {
        score = 0;
        timeScale = 1.0;
    }

  private:
    int score;
    float timeScale;
    cocos2d::Vec3 accelerometerReference;
};

}
