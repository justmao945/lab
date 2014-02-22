#include "HelloWorldScene.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    // Size visibleSize = Director::getInstance()->getVisibleSize();
    // Point origin = Director::getInstance()->getVisibleOrigin();

    mBatchNode = SpriteBatchNode::create("Sprites.pvr.ccz");
    addChild(mBatchNode);
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Sprites.plist");
    
    mShip = Sprite::createWithSpriteFrameName("SpaceFlier_sm_1.png");
    auto winSize = Director::getInstance()->getWinSize();
    mShip->setPosition(winSize.width * 0.1, winSize.height * 0.5);
    mBatchNode->addChild(mShip, 1);
    
    return true;
}


void HelloWorld::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
