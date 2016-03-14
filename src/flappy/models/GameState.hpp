#pragma once

struct GameState {

    GameState(): score(0) {}

    void reset() {
        score = 0;
    }

    int score;
};
