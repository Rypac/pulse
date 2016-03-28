#pragma once

#include "cocos2d.h"

namespace flappy {

struct GameState {

    struct TimeScale {

        TimeScale(): player(1.0), environment(1.0) {}

        float getPlayer() const { return player; }
        void setPlayer(float scale) { player = scale; }
        float getEnvironment() const { return environment; }
        void setEnvironment(float scale) { environment = scale; }

      private:
        float player;
        float environment;
    };

    GameState(): score(0), timeScale(TimeScale{}), accelerometerReference(cocos2d::Vec3{1, 0, 1}) {}

    int currentScore() const { return score; };
    void addToScore() { score++; };

    cocos2d::Vec3 calibratedAccelerometerOffset() const { return accelerometerReference; };
    void calibrateAccelerometer(cocos2d::Vec3 offset) { accelerometerReference = offset; };

    TimeScale& getTimeScale() { return timeScale; }

    void reset() {
        score = 0;
        timeScale = TimeScale{};
    }

  private:
    int score;
    TimeScale timeScale;
    cocos2d::Vec3 accelerometerReference;
};

}
