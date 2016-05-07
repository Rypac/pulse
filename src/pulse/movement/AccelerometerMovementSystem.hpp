#pragma once

#include "cocos2d.h"
#include "pulse/movement/MovementSystem.hpp"
#include "pulse/models/Accelerometer.hpp"

namespace pulse {

class AccelerometerMovementSystem : public cocos2d::EventListenerAcceleration, public MovementSystem {
  public:
    static AccelerometerMovementSystem* create(Accelerometer* accelerometer);

  CC_CONSTRUCTOR_ACCESS:
    AccelerometerMovementSystem(Accelerometer* accelerometer): accelerometer_{accelerometer} {}

    virtual bool init();

  private:
    void onAccelerationDetected(cocos2d::Acceleration*, cocos2d::Event*);

    Accelerometer* accelerometer_;
    cocos2d::Acceleration previousReading;
};

}
