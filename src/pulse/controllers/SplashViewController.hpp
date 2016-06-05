#pragma once

#include "pulse/controllers/ViewController.hpp"
#include "pulse/views/SplashView.hpp"

namespace pulse {

class SplashViewController : public ViewController<SplashView> {
  public:
    using Callback = std::function<void(SplashViewController* self)>;
    Callback onDismissed;

    virtual void viewDidAppear() override;
};

}
