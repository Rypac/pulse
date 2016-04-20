#include "pulse/scenes/TitleScene.hpp"
#include "pulse/utilities/geometry.hpp"

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
    title->setScale(30.0f);
    addChild(title);

    addTouchListeners();

    return true;
}

void TitleScene::onEnter() {
    GameScene::onEnter();
    titleAnimation = titleScreenAnimation();
    title->runAction(titleAnimation);
    scheduleUpdate();
}

void TitleScene::update(float dt) {
    titleAnimation->step(dt * animationSpeedScale);
}

Action* TitleScene::titleScreenAnimation() {
    const auto animation = Animation3D::create("animations/title.c3b");
    const auto animate = Animate3D::create(animation);
    animate->setSpeed(0.001);
    const auto finish = CallFunc::create([this]() { onSceneDismissed(this); });
    const auto delay = DelayTime::create(1.0);
    return Sequence::create(animate, delay, finish, nullptr);
}

void TitleScene::addTouchListeners() {
    const auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = CC_CALLBACK_2(TitleScene::onTouchBegan, this);
    listener->onTouchEnded = CC_CALLBACK_2(TitleScene::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

bool TitleScene::onTouchBegan(Touch *touch, Event *unused_event) {
    animationSpeedScale = 3000.0;
    return true;
}

void TitleScene::onTouchEnded(Touch *touch, Event *unused_event) {
    animationSpeedScale = -1500.0;
}
