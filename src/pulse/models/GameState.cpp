#include "pulse/models/GameState.hpp"
#include "pulse/utilities/Callback.hpp"

using namespace pulse;

GameState::GameState(const GameOptions& options)
: options_{options}
, gameIsOver_{true}
, normalTimeScale_{1.0, 1.0}
, score_{0}
, timeMode_{TimeMode::Normal}
, accelerometer_{30}
{}

int GameState::currentScore() const {
    return score_;
}

void GameState::incrementScore() {
    score_++;
}

void GameState::gameOver() {
    gameIsOver_ = true;
}

bool GameState::isGameOver() const {
    return gameIsOver_;
}

Accelerometer& GameState::accelerometer() {
    return accelerometer_;
}

GameState::TimeMode GameState::currentMode() const {
    return timeMode_;
}

void GameState::enterMode(TimeMode mode) {
    timeMode_ = mode;
    safe_callback(onTimeModeChanged, mode);
}

float GameState::playerTimeScale() const {
    return timeScale().player;
}

float GameState::environmentTimeScale() const {
    return timeScale().environment;
}

void GameState::reset() {
    score_ = 0;
    timeMode_ = TimeMode::Normal;
    gameIsOver_ = false;
    accelerometer_.reset();
}

const TimeScale& GameState::timeScale() const {
    return timeMode_ == TimeMode::Normal ? normalTimeScale_ : options_.slowMotionTimeScale;
}
