#include "pulse/controllers/SplashViewController.hpp"
#include "pulse/ui/Resources.hpp"
#include "pulse/utilities/Callback.hpp"
#include "SimpleAudioEngine.h"

using namespace pulse;
using namespace cocos2d;

void SplashViewController::viewDidAppear() {
    const auto logoAnimation = Animate::create(view()->animation());
    const auto logoAudio = CallFunc::create([]() {
        const auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
        audio->playEffect(Resources::Audio::Intro::Long, false, 1.0f, 1.0f, 1.0f);
    });
    const auto introAnimation = Spawn::createWithTwoActions(logoAnimation, logoAudio);
    const auto onCompletion = CallFunc::create([this]() {
        safe_callback(onDismissed, this);
    });

    view()->image()->runAction(Sequence::createWithTwoActions(introAnimation, onCompletion));
}
