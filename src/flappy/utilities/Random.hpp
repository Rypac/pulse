#pragma once

namespace flappy { namespace random {

int between(int min, int max) {
    std::random_device seed;
    std::default_random_engine randomEngine(seed());
    std::uniform_int_distribution<int> uniformDistribution(min, max);
    return uniformDistribution(randomEngine);
}
    
} }
