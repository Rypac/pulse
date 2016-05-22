#include "pulse/scenes/TitleScene.hpp"
#include "pulse/actions/AnimatedBackground.hpp"
#include "pulse/utilities/Geometry.hpp"
#include "pulse/ui/Resources.hpp"

using namespace pulse;
using namespace cocos2d;

namespace AnimationStep {
    const auto Backwards = -3000.0f;
    const auto Forwards = 3000.0f;
}

TitleScene::TitleScene(): animationStep{AnimationStep::Backwards} {}

TitleScene::~TitleScene() {
    CC_SAFE_RELEASE(titleAnimation);
    CC_SAFE_RELEASE(title);
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
    addTitleAnimation();
    addPlayButton();
    return true;
}

void TitleScene::addBackground() {
    const auto particles = ParticleSystemQuad::create(Resources::Particles::AmbientBackground);
    runAction(AnimatedBackground::create(particles));
}

void TitleScene::addTitle() {
    title = Sprite3D::create(Resources::Animations::Title);
    title->retain();
    title->setPosition(geometry::centerOf(sceneFrame()));
    addChild(title);
}

void TitleScene::addTitleAnimation() {
    const auto animation = Animation3D::create(Resources::Animations::Title);
    const auto animate = Animate3D::create(animation);
    animate->setSpeed(0.001);
    const auto finish = CallFunc::create([this]() { onSceneDismissed(this); });

    titleAnimation = Sequence::create(animate, finish, nullptr);
    titleAnimation->retain();
}

void TitleScene::addPlayButton() {
    const auto playButton = ui::Button::create(Resources::Buttons::Play);
    playButton->setAnchorPoint(Vec2{1.0, 0.5});
    playButton->setPosition(Vec2{sceneFrame().getMaxX(), sceneFrame().getMinY() + 120});
    addChild(playButton);

    playButton->onTouchBegan = [this](auto ref) { animationStep = AnimationStep::Forwards; };
    playButton->onTouchEnded = [this](auto ref) { animationStep = AnimationStep::Backwards; };
    playButton->onTouchCancelled = [this](auto ref) { animationStep = AnimationStep::Backwards; };

    const auto particles = ParticleSystemQuad::create(Resources::Particles::ButtonBackground);
    playButton->runAction(AnimatedBackground::create(particles));
}

void TitleScene::onEnter() {
    GameScene::onEnter();
    title->runAction(titleAnimation);
    scheduleUpdate();
}

void TitleScene::update(float dt) {
    if (titleAnimation->isDone()) {
        return;
    }

    const auto elapsed = titleAnimation->getElapsed() / titleAnimation->getDuration();
    if (elapsed > 0 || animationStep > 0) {
        const auto easeOut = [](auto elapsed) { return 1.0 - elapsed + 0.25; };
        const auto animationCurve = animationStep > 0 ? easeOut(elapsed) : 1.0;
        float step = dt * animationStep * animationCurve;
        const auto remaining = titleAnimation->getDuration() - titleAnimation->getElapsed();
        titleAnimation->step(std::min(remaining, step));
    }
}
