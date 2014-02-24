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

    // Load sprites to global cache
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Sprites.plist");
    
    // create from cache
    mShip = Sprite::createWithSpriteFrameName("SpaceFlier_sm_1.png");
    assert(mShip);
    auto winSize = Director::getInstance()->getWinSize();
    mShip->setPosition(winSize.width * 0.1, winSize.height * 0.5);

    // add batch node for effcient
    mBatchNode = SpriteBatchNode::create("Sprites.pvr.ccz");
    assert(mBatchNode);
    mBatchNode->addChild(mShip, 1);
    addChild(mBatchNode);
    
    // create background
    mBackgroundNode = ParallaxNode::create();
    addChild(mBackgroundNode, -1);
    
    // create all sprites on background
    mSpaceDust1 = Sprite::create("bg_front_spacedust.png");
    assert(mSpaceDust1);
    mSpaceDust2 = Sprite::create("bg_front_spacedust.png");
    assert(mSpaceDust2);
    mPlanetSunrise = Sprite::create("bg_planetsunrise.png");
    assert(mPlanetSunrise);
    mGalaxy = Sprite::create("bg_galaxy.png");
    assert(mGalaxy);
    mSpacialAnomaly  = Sprite::create("bg_spacialanomaly.png");
    assert(mSpacialAnomaly);
    mSpacialAnomaly2 = Sprite::create("bg_spacialanomaly2.png");
    assert(mSpacialAnomaly2);
    
    // determine bg speed and dust speed
    Point bg_speed(0.05, 0.05), dust_speed(0.1, 0.1);
    
    // add children to ParallaxNode
    mBackgroundNode->addChild(mSpaceDust1, 0, dust_speed, Point(0, winSize.height/2));
    mBackgroundNode->addChild(mSpaceDust2, 0, dust_speed, Point(mSpaceDust1->getContentSize().width, winSize.height / 2));
    
    mBackgroundNode->addChild(mGalaxy, -1, bg_speed, Point(0, winSize.height * .7));
    mBackgroundNode->addChild(mPlanetSunrise, -1, bg_speed, Point(600, winSize.height * 0));
    mBackgroundNode->addChild(mSpacialAnomaly, -1, bg_speed, Point(900, winSize.height * .3));
    mBackgroundNode->addChild(mSpacialAnomaly2, -1, bg_speed, Point(1500, winSize.height * .9));
    
    scheduleUpdate();
    return true;
}


void HelloWorld::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

void HelloWorld::update(float dt) {
    Point scroll_bg_hori(-1000, 0);
    mBackgroundNode->setPosition(mBackgroundNode->getPosition() + scroll_bg_hori * dt);
}

