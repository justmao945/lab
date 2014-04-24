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
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Point origin = Director::getInstance()->getVisibleOrigin();

    // position the sprite on the center of the screen
    Point center(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y);
    
    // create solid background
    auto bg = LayerColor::create(Color4B(170,170,170,255), visibleSize.width, visibleSize.height);
    bg->setPosition(origin);
    this->addChild(bg, -1);
    
    // 1x1 white
    auto blanks = SpriteBatchNode::create("blank.png");
    this->addChild(blanks);
    
    return true;
}