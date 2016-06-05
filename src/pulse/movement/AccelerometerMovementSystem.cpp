#include "pulse/movement/AccelerometerMovementSystem.hpp"
#include "pulse/utilities/Acceleration.hpp"
#include "pulse/utilities/Rotation.hpp"
#include "pulse/extensions/Vec.hpp"

using namespace pulse;
using namespace cocos2d;

AccelerometerMovementSystem* AccelerometerMovementSystem::create(Accelerometer* accelerometer) {
    AccelerometerMovementSystem* movement = new (std::nothrow) AccelerometerMovementSystem(accelerometer);
    if (movement && movement->init()) {
        movement->autorelease();
        return movement;
    }
    delete movement;
    return nullptr;
}

bool AccelerometerMovementSystem::init() {
    Device::setAccelerometerEnabled(true);
    Device::setAccelerometerInterval(1.0f / 60.0f);
    return EventListenerAcceleration::init(CC_CALLBACK_2(AccelerometerMovementSystem::onAccelerationDetected, this));
}

void AccelerometerMovementSystem::onAccelerationDetected(Acceleration *acceleration, Event *event) {
    const auto smoothedReading = accelerometer::filter(*acceleration, previousReading);
    previousReading = smoothedReading;
    const auto currentAcceleration = Vec3(smoothedReading.x, smoothedReading.y, smoothedReading.z);
    const auto rotationAngle = rotation::angle(currentAcceleration);

    if (!accelerometer_->isCalibrated()) {
        accelerometer_->calibrate(rotationAngle);
        return;
    }

    if (onMovement) {
        const auto relativeAngle = rotationAngle - *accelerometer_->offset();
        const auto velocity = vec::scaleBy(relativeAngle, accelerometer_->sensitivity());
        onMovement(velocity);
    }
}
