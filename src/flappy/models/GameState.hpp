#pragma once

namespace flappy {

struct GameState {

    GameState(): score(0) {}

    int currentScore() const { return score; };
    void addToScore() { score++; };
    void reset() { score = 0; }

  private:
    int score;
};

}
