#pragma once

#include "cocos2d.h"
#include "pulse/movement/MovementSystem.hpp"
#include "pulse/models/Accelerometer.hpp"

namespace pulse {

class AccelerometerMovementSystem : public cocos2d::EventListenerAcceleration, public MovementSystem {
  public:
    AccelerometerMovementSystem(Accelerometer* accelerometer);

    cocos2d::Vec3 sample(const cocos2d::Acceleration& acceleration);

  private:
    Accelerometer* accelerometer_;
    cocos2d::Acceleration previousReading_;
};

}
