#include "pulse/movement/AccelerometerMovementSystem.hpp"
#include "pulse/extensions/Vec.hpp"
#include "pulse/utilities/Acceleration.hpp"
#include "pulse/utilities/Callback.hpp"
#include "pulse/utilities/Rotation.hpp"

using namespace pulse;
using namespace cocos2d;

AccelerometerMovementSystem::AccelerometerMovementSystem(Accelerometer* accelerometer)
: accelerometer_{accelerometer} {
    init([this](auto acceleration, auto event) {
        const auto rotationAngle = this->sample(*acceleration);

        if (accelerometer_->isCalibrated()) {
            const auto relativeAngle = rotationAngle - *accelerometer_->offset();
            const auto velocity = vec::scaleBy(relativeAngle, accelerometer_->sensitivity());
            safe_callback(onMovement, velocity);
        } else {
            accelerometer_->calibrate(rotationAngle);
        }
    });
}

Vec3 AccelerometerMovementSystem::sample(const Acceleration& accelerationReading) {
    const auto smoothedReading = accelerometer::filter(accelerationReading, previousReading_);
    previousReading_ = smoothedReading;
    const auto acceleration = Vec3(smoothedReading.x, smoothedReading.y, smoothedReading.z);
    return rotation::angle(acceleration);
}
