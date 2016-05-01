#include "pulse/scenes/TitleScene.hpp"
#include "pulse/utilities/Geometry.hpp"

using pulse::TitleScene;
using namespace cocos2d;

TitleScene* TitleScene::create() {
    TitleScene *scene = new (std::nothrow) TitleScene();
    if (scene && scene->init()) {
        scene->autorelease();
        return scene;
    }
    delete scene;
    return nullptr;
}

bool TitleScene::init() {
    if (!GameScene::init()) {
        return false;
    }

    title = Sprite3D::create("animations/title.c3b");
    title->setPosition(geometry::centerOf(frame));
    title->setScale(35);
    addChild(title);

    addTouchListeners();

    return true;
}

void TitleScene::onEnter() {
    GameScene::onEnter();
    animationStep = 0;
    titleAnimation = titleScreenAnimation();
    title->runAction(titleAnimation);
    scheduleUpdate();
}

void TitleScene::update(float dt) {
    const auto elapsed = titleAnimation->getElapsed() / titleAnimation->getDuration();
    if (elapsed > 0 || animationStep > 0) {
        const auto easeOut = [](auto elapsed) { return 1.0 - elapsed + 0.25; };
        const auto animationCurve = animationStep > 0 ? easeOut(elapsed) : 1.0;
        float step = dt * animationStep * animationCurve;
        const auto remaining = titleAnimation->getDuration() - titleAnimation->getElapsed();
        titleAnimation->step(std::min(remaining, step));
    }
}

Sequence* TitleScene::titleScreenAnimation() {
    const auto animation = Animation3D::create("animations/title.c3b");
    const auto animate = Animate3D::create(animation);
    animate->setSpeed(0.001);
    const auto finish = CallFunc::create([this]() { onSceneDismissed(this); });
    return Sequence::create(animate, finish, nullptr);
}

void TitleScene::addTouchListeners() {
    const auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = [this](auto touch, auto event) {
        animationStep = 3000.0;
        return true;
    };
    listener->onTouchEnded = [this](auto touch, auto event) {
        animationStep = -3000.0;
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}
