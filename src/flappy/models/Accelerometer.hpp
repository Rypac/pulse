#pragma once

#include "cocos2d.h"
#include "cpputils/optional.hpp"

namespace flappy {

struct Accelerometer {
  public:
    Accelerometer(int requiredCalibrationReadings): requiredCalibrationReadings{requiredCalibrationReadings} {}

    void calibrate(const cocos2d::Vec3& currentReading) {
        aggregateReading.x += currentReading.x;
        aggregateReading.y += currentReading.y;
        aggregateReading.z += currentReading.z;
        totalReadings++;
    }

    void reset() {
        totalReadings = 0;
        aggregateReading = cocos2d::Vec3{};
    }

    bool isCalibrated() const {
        return totalReadings >= requiredCalibrationReadings;
    }

    std::optional<cocos2d::Vec3> offset() const {
        return isCalibrated() ? std::make_optional(averageReading()) : std::nullopt;
    }

private:
    cocos2d::Vec3 averageReading() const {
        return aggregateReading * (1.0 / totalReadings);
    }

    const int requiredCalibrationReadings;
    int totalReadings{0};
    cocos2d::Vec3 aggregateReading{};
};

}
