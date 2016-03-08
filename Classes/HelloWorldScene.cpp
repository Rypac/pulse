#include "HelloWorldScene.h"

#include <gsl/gsl.h>
#include <range/v3/all.hpp>
#include <vector>

USING_NS_CC;

Scene* HelloWorld::createScene() {
    auto scene = Scene::createWithPhysics();
    scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
    auto layer = HelloWorld::create();
    scene->addChild(layer);
    return scene;
}

static inline Vec2 centerOf(Rect frame) {
    return Vec2(frame.origin.x + frame.size.width / 2, frame.origin.y + frame.size.height / 2);
}

gsl::owner<Node*> createSceneBorder(Rect frame) {
    auto scene = Node::create();
    auto borderPhysics = PhysicsBody::createEdgeBox(frame.size, PhysicsMaterial(0.1f, 1.0f, 0.0f));
    scene->setPhysicsBody(borderPhysics);
    scene->setPosition(centerOf(frame));
    return scene;
}

gsl::owner<Sprite*> createHero() {
    auto hero = Sprite::create("HelloWorld.png");
    auto physicsBody = PhysicsBody::createBox(hero->getBoundingBox().size, PhysicsMaterial(0.1f, 1.0f, 0.0f));
    physicsBody->setDynamic(true);
    hero->addComponent(physicsBody);
    return hero;
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

    hitDetector = createHero();
    hitDetector->setPosition(centerOf(frame));
    this->addChild(hitDetector);

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
    auto shapeToNode = [](PhysicsShape* shape) -> Node* { return shape->getBody()->getNode(); };
    return shapes | ranges::view::transform(shapeToNode);
}

bool containsNode(const std::vector<Node*>& nodes, const gsl::not_null<Node*> targetNode) {
    return ranges::any_of(nodes, [=](auto node) { return node == targetNode; });
}

bool HelloWorld::onTouchBegan(Touch* touch, Event* event) {
    auto heroBody = hitDetector->getPhysicsBody();
    if (heroBody) {
        heroBody->setVelocity(Vec2(0, 0));
        heroBody->applyImpulse(Vec2(0, 100000));
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
