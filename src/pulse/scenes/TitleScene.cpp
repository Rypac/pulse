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
    title->setScale(20.f);
    title->setPosition(geometry::centerOf(frame));
    addChild(title, 1);

    return true;
}

void TitleScene::onEnter() {
    GameScene::onEnter();
    title->runAction(titleScreenAnimation());
}

Action* TitleScene::titleScreenAnimation() {
    const auto animation = Animation3D::create("animations/title.c3b");
    const auto animate = Animate3D::create(animation);
    const auto finish = CallFunc::create([this]() { onSceneDismissed(this); });
    return Sequence::create(animate, finish, nullptr);
}
