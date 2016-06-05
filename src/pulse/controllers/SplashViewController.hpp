#pragma once

#include "pulse/controllers/ViewController.hpp"
#include "pulse/views/SplashView.hpp"

namespace pulse {

class SplashViewController : public ViewController<SplashView> {
  public:
    virtual void viewDidAppear() override;
};

}
