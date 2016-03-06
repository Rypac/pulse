#include "HelloWorldScene.h"

#include <gsl/gsl.h>
#include <range/v3/all.hpp>
#include <rdl/vector.hpp>

USING_NS_CC;

Scene* HelloWorld::createScene() {
    auto scene = Scene::createWithPhysics();
    auto layer = HelloWorld::create();
    scene->addChild(layer);
    return scene;
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
                                frame.origin.y + closeItem->getContentSize().height/2));

    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    auto label = Label::createWithTTF("Hello World", "fonts/Marker Felt.ttf", 24);
    label->setPosition(Vec2(frame.origin.x + frame.size.width / 2,
                            frame.origin.y + frame.size.height - label->getContentSize().height));
    this->addChild(label, 1);

    hitDetectionLabel = Label::createWithTTF("Touch anywhere!", "fonts/Marker Felt.ttf", 20);
    hitDetectionLabel->setPosition(Vec2(frame.origin.x + frame.size.width / 2,
                                        1.2 * hitDetectionLabel->getContentSize().height));
    this->addChild(hitDetectionLabel);

    hitDetector = Sprite::create("HelloWorld.png");
    hitDetector->setPosition(Vec2(frame.size.width / 2 + frame.origin.x, frame.size.height / 2 + frame.origin.y));
    this->addChild(hitDetector);

    auto physicsBody = PhysicsBody::createBox(hitDetector->getContentSize(), PhysicsMaterial(0.1f, 1.0f, 0.0f));
    physicsBody->setDynamic(false);
    hitDetector->addComponent(physicsBody);

    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan, this);
    touchListener->onTouchEnded = CC_CALLBACK_2(HelloWorld::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

    return true;
}

rdl::Vector<Node*> nodesAtPosition(Vec2 position) {
    auto scene = Director::getInstance()->getRunningScene();
    auto shapes = scene->getPhysicsWorld()->getShapes(position);
    auto shapeToNode = [](PhysicsShape* shape) -> Node* { return shape->getBody()->getNode(); };
    return shapes | ranges::view::transform(shapeToNode);
}

bool containsNode(const rdl::Vector<Node*>& nodes, const gsl::not_null<Node*> targetNode) {
    return ranges::any_of(nodes, [=](auto node) { return node == targetNode; });
}

bool HelloWorld::onTouchBegan(Touch* touch, Event* event) {
    auto touchPosition = this->convertTouchToNodeSpace(touch);
    const auto touchedNodes = nodesAtPosition(touchPosition);

    if (containsNode(touchedNodes, hitDetector)) {
        hitDetectionLabel->setColor(Color3B::GREEN);
        hitDetectionLabel->setString("Ohhh, you touched me!");
    } else {
        hitDetectionLabel->setColor(Color3B::RED);
        hitDetectionLabel->setString("Haha, missed me!");
    }

    return true;
}

void HelloWorld::onTouchEnded(Touch* touch, Event* event) {
    hitDetectionLabel->setColor(Color3B::WHITE);
    hitDetectionLabel->setString("Touch anywhere!");
}

void HelloWorld::menuCloseCallback(Ref* pSender) {
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
