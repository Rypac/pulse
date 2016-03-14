#pragma once

struct GameState {

    enum class Status {
        Running, Paused, Stopped
    };

    GameState(): status(Status::Stopped), score(0) {}

    void reset() {
        status = Status::Stopped;
        score = 0;
    }

    Status status;
    int score;
};
