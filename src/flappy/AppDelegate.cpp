#include "flappy/AppDelegate.h"
#include "flappy/scenes/FlappyBirdScene.hpp"

#include <algorithm>

using namespace cocos2d;
using namespace flappy;

namespace DisplayResolution {
    const auto Design = Size(960, 640);
    const auto Small = Size(960, 640);
    const auto Medium = Size(1024, 768);
    const auto Large = Size(2048, 1536);
}

AppDelegate::AppDelegate() {}

AppDelegate::~AppDelegate() {}

void AppDelegate::initGLContextAttrs() {
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};
    GLView::setGLContextAttrs(glContextAttrs);
}

static int register_all_packages() {
    return 0;
}

Size displayResolutionForFrame(Size frame) {
    if (frame.height > DisplayResolution::Medium.height) {
        return DisplayResolution::Large;
    } else if (frame.height > DisplayResolution::Small.height) {
        return DisplayResolution::Medium;
    } else {
        return DisplayResolution::Small;
    }
}

float contentScaleFactorForFrame(Size frame) {
    auto resolution = displayResolutionForFrame(frame);
    auto scaledHeight = resolution.height / DisplayResolution::Design.height;
    auto scaledWidth = resolution.width / DisplayResolution::Design.width;
    return std::min(scaledHeight, scaledWidth);
}

bool AppDelegate::applicationDidFinishLaunching() {
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if (!glview) {
        glview = GLViewImpl::create("FlappyBird");
        director->setOpenGLView(glview);
    }

    director->setDisplayStats(true);
    director->setAnimationInterval(1.0 / 60);

    glview->setDesignResolutionSize(DisplayResolution::Design.width, DisplayResolution::Design.height, ResolutionPolicy::NO_BORDER);
    auto frame = glview->getFrameSize();
    director->setContentScaleFactor(contentScaleFactorForFrame(frame));

    register_all_packages();

    auto scene = FlappyBirdScene::createScene();
    director->runWithScene(scene);

    return true;
}

void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();
}

void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();
}
