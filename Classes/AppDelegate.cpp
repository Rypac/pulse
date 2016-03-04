#include "AppDelegate.h"
#include "HelloWorldScene.h"

USING_NS_CC;

namespace DisplayResolution {
    static const cocos2d::Size design = cocos2d::Size(480, 320);
    static const cocos2d::Size small = cocos2d::Size(480, 320);
    static const cocos2d::Size medium = cocos2d::Size(1024, 768);
    static const cocos2d::Size large = cocos2d::Size(2048, 1536);
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

cocos2d::Size displayResolutionForFrame(Size frame) {
    if (frame.height > DisplayResolution::medium.height) {
        return DisplayResolution::large;
    } else if (frame.height > DisplayResolution::small.height) {
        return DisplayResolution::medium;
    } else {
        return DisplayResolution::small;
    }
}

float contentScaleFactorForFrame(Size frame) {
    auto resolution = displayResolutionForFrame(frame);
    auto scaledHeight = resolution.height / DisplayResolution::design.height;
    auto scaledWidth = resolution.width / DisplayResolution::design.width;
    return MIN(scaledHeight, scaledWidth);
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

    glview->setDesignResolutionSize(DisplayResolution::design.width, DisplayResolution::design.height, ResolutionPolicy::NO_BORDER);
    auto frame = glview->getFrameSize();
    director->setContentScaleFactor(contentScaleFactorForFrame(frame));

    register_all_packages();

    auto scene = HelloWorld::createScene();
    director->runWithScene(scene);

    return true;
}

void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();
}

void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();
}
