#pragma once

#include "cocos2d.h"
#include "pulse/extensions/Optional.hpp"

namespace pulse {

struct Accelerometer {

    Accelerometer(int requiredCalibrationReadings): requiredCalibrationReadings{requiredCalibrationReadings} {}

    void calibrate(const cocos2d::Vec3& currentReading) {
        aggregateReading += currentReading;
        totalReadings++;
    }

    void reset() {
        totalReadings = 0;
        aggregateReading = cocos2d::Vec3::ZERO;
    }

    cocos2d::Vec3 sensitivity() const {
        return cocos2d::Vec3{2500, 1500, 0};
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
