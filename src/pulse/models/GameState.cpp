#include "pulse/models/GameState.hpp"
#include "pulse/utilities/Callback.hpp"

using namespace pulse;

GameState::GameState(const GameOptions& options)
: options_{options}
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
    safe_callback(onScoreChanged);
}

void GameState::newGame() {
    reset();
    safe_callback(onNewGame);
}

void GameState::gameOver() {
    safe_callback(onGameOver);
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

float GameState::obstacleSpeed() const {
    if (options_.gameMode == GameMode::FreePlay) {
        return options_.obstacleSpeed;
    }
    const auto multiplier = floor(currentScore() / options_.obstacleDefeatedThreshold);
    return options_.obstacleSpeed - options_.obstacleSpeedStep * multiplier;
}

float GameState::obstacleFrequency() const {
    if (options_.gameMode == GameMode::FreePlay) {
        return options_.obstacleFrequency;
    }
    const auto multiplier = floor(currentScore() / options_.obstacleDefeatedThreshold);
    return options_.obstacleFrequency - options_.obstacleFrequencyStep * multiplier;
}

void GameState::reset() {
    score_ = 0;
    timeMode_ = TimeMode::Normal;
    accelerometer_.reset();
}

const TimeScale& GameState::timeScale() const {
    if (options_.gameMode == GameMode::Reverse) {
        return timeMode_ == TimeMode::Normal ? options_.reverseModeTimeScale : normalTimeScale_;
    }
    return timeMode_ == TimeMode::Normal ? normalTimeScale_ : options_.slowMotionTimeScale;
}
