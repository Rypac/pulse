#pragma once

namespace flappy {

struct GameState {

    GameState(): score(0), accelerometerOffset(0) {}

    int currentScore() const { return score; };
    void addToScore() { score++; };
    void reset() { score = 0; }

    double calibratedAccelerometerOffset() const { return accelerometerOffset; };
    void calibrateAccelerometer(double offset) { accelerometerOffset = offset; };

  private:
    int score;
    double accelerometerOffset;
};

}
