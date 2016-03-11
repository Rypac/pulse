#include "HelloWorldScene.h"

#include <gsl/gsl.h>
#include <range/v3/all.hpp>
#include <vector>

USING_NS_CC;

using gsl::owner;
using gsl::not_null;

owner<Scene*> HelloWorld::createScene() {
    auto scene = Scene::createWithPhysics();
    scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
    auto layer = HelloWorld::create();
    scene->addChild(layer);
    return scene;
}

static inline Vec2 centerOf(Rect frame) {
    return Vec2(frame.origin.x + frame.size.width / 2, frame.origin.y + frame.size.height / 2);
}

owner<Node*> createSceneBorder(Rect frame) {
    auto scene = Node::create();
    auto borderPhysics = PhysicsBody::createEdgeBox(frame.size);
    scene->setPhysicsBody(borderPhysics);
    scene->setPosition(centerOf(frame));
    return scene;
}

owner<Sprite*> createFlappy() {
    auto flappy = Sprite::create();
    flappy->setTextureRect(Rect(0, 0, 30, 30));
    flappy->setColor(Color3B::WHITE);

    auto physicsBody = PhysicsBody::createBox(flappy->getBoundingBox().size, PhysicsMaterial(10.0f, 0.5f, 0.0f));
    physicsBody->setDynamic(true);
    flappy->addComponent(physicsBody);
    return flappy;
}

owner<Sprite*> createColumn(Rect sceneFrame) {
    auto spriteFrame = Rect(0, 0, 50, sceneFrame.size.height);
    auto column = Sprite::create();
    column->setTextureRect(spriteFrame);
    column->setColor(Color3B::BLUE);

    auto physicsBody = PhysicsBody::createBox(column->getBoundingBox().size, PhysicsMaterial(0.1f, 0.0f, 0.0f));
    physicsBody->setDynamic(true);
    physicsBody->applyForce(Vec2(-50000, 0));
    column->addComponent(physicsBody);

    return column;
}

bool HelloWorld::init() {
    if (!Layer::init()) {
        return false;
    }

    auto director = Director::getInstance();
    auto frame = Rect(director->getVisibleOrigin(), director->getVisibleSize());

    auto closeItem = MenuItemImage::create(
        "CloseNormal.png",
        "CloseSelected.png",
        CC_CALLBACK_1(HelloWorld::menuCloseCallback, this)
    );
    closeItem->setPosition(Vec2(frame.origin.x + frame.size.width - closeItem->getContentSize().width / 2,
                                frame.origin.y + closeItem->getContentSize().height / 2));

    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    flappy = createFlappy();
    flappy->setPosition(centerOf(frame));
    this->addChild(flappy);

    auto column = createColumn(frame);
    column->setPosition(Vec2(frame.origin.x + frame.size.width - column->getContentSize().width / 2,
                             frame.origin.y + frame.size.height - column->getContentSize().height / 2));
    this->addChild(column);

    auto sceneBorder = createSceneBorder(frame);
    this->addChild(sceneBorder);

    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan, this);
    touchListener->onTouchEnded = CC_CALLBACK_2(HelloWorld::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

    return true;
}

std::vector<Node*> nodesAtPosition(Vec2 position) {
    auto scene = Director::getInstance()->getRunningScene();
    auto shapes = scene->getPhysicsWorld()->getShapes(position);
    auto toNode = [](auto shape) { return shape->getBody()->getNode(); };
    return shapes | ranges::view::transform(toNode) | ranges::to_vector;
}

bool containsNode(const std::vector<Node*>& nodes, const not_null<Node*> targetNode) {
    return ranges::any_of(nodes, [=](auto node) { return node == targetNode; });
}

bool HelloWorld::onTouchBegan(Touch* touch, Event* event) {
    auto flappyBody = flappy->getPhysicsBody();
    if (flappyBody) {
        flappyBody->setVelocity(Vec2(flappyBody->getVelocity().x, 0));
        flappyBody->applyImpulse(Vec2(0, 1000000));
    }
    return true;
}

void HelloWorld::onTouchEnded(Touch* touch, Event* event) {
}

void HelloWorld::menuCloseCallback(Ref* pSender) {
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
