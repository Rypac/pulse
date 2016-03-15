#pragma once

#include "cocos2d.h"

namespace flappy {

struct GameState {

    GameState(): score(0), accelerometerReference(cocos2d::Vec3{1, 0, 1}) {}

    int currentScore() const { return score; };
    void addToScore() { score++; };
    void reset() { score = 0; }

    cocos2d::Vec3 calibratedAccelerometerOffset() const { return accelerometerReference; };
    void calibrateAccelerometer(cocos2d::Vec3 offset) { accelerometerReference = offset; };

  private:
    int score;
    cocos2d::Vec3 accelerometerReference;
};

}
