#include "pulse/scenes/TitleScene.hpp"
#include "pulse/utilities/Geometry.hpp"
#include "pulse/ui/Resources.hpp"

using namespace pulse;
using namespace cocos2d;

namespace AnimationStep {
    const auto Backwards = -3000.0f;
    const auto Forwards = 3000.0f;
}

TitleScene* TitleScene::create() {
    const auto scene = new (std::nothrow) TitleScene();
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
    addBackground();
    addTitle();
    addTouchListeners();
    return true;
}

void TitleScene::addBackground() {
    const auto particles = ParticleSystemQuad::create(Resources::Particles::AmbientBackground);
    particles->setPosition(sceneFrame().origin);
    particles->setSourcePosition(geometry::centerOf(sceneFrame()));
    particles->setPosVar(sceneFrame().size / 2);
    addChild(particles, -1);
}

void TitleScene::addTitle() {
    title = Sprite3D::create(Resources::Animations::Title);
    title->setPosition(geometry::centerOf(sceneFrame()));
    addChild(title);
}

void TitleScene::addTouchListeners() {
    const auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = [this](auto touch, auto event) {
        animationStep = AnimationStep::Forwards;
        return true;
    };
    listener->onTouchEnded = [this](auto touch, auto event) {
        animationStep = AnimationStep::Backwards;
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void TitleScene::onEnter() {
    GameScene::onEnter();
    animationStep = AnimationStep::Backwards;
    titleAnimation = createTitleAnimation();
    title->runAction(titleAnimation);
    scheduleUpdate();
}

Sequence* TitleScene::createTitleAnimation() {
    const auto animation = Animation3D::create(Resources::Animations::Title);
    const auto animate = Animate3D::create(animation);
    animate->setSpeed(0.001);
    const auto finish = CallFunc::create([this]() { onSceneDismissed(this); });
    return Sequence::create(animate, finish, nullptr);
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
