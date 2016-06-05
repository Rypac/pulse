#pragma once

#include "cocos2d.h"
#include "pulse/movement/MovementSystem.hpp"
#include "pulse/models/Accelerometer.hpp"

namespace pulse {

class AccelerometerMovementSystem : public cocos2d::EventListenerAcceleration, public MovementSystem {
  public:
    AccelerometerMovementSystem(Accelerometer* accelerometer);

  private:
    Accelerometer* accelerometer_;
    cocos2d::Acceleration previousReading_;
};

}
