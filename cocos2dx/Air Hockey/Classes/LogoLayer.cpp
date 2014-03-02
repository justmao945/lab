//
//  LogoLayer.cpp
//  Air Hockey
//
//  Created by Jianjun on 2/3/14.
//
//

#include "GameLayer.h"
#include "LogoLayer.h"

USING_NS_CC;

Scene* LogoLayer::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = LogoLayer::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool LogoLayer::init()
{
    if ( !Layer::init() )
    {
        return false;
    }

    Sprite* logo = Sprite::create("logo.png");
    
    Director* director = Director::getInstance();
    Size winSize = director->getWinSize();
    logo->setPosition(Point(winSize.width * 0.5, winSize.height * 0.5));
    addChild(logo);
    
    EventListenerTouchOneByOne* listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = CC_CALLBACK_2(LogoLayer::onTouchBegan, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, logo);
    
    return true;
}

bool LogoLayer::onTouchBegan(Touch* touch, Event* unused_event) {
    
    // create a scene. it's an autorelease object
    auto scene = GameLayer::createScene();
    
    // run
    Director::getInstance()->replaceScene(scene);
    
    return true;
}

