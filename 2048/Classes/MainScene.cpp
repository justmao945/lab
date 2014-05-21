
#include <algorithm>
#include "Config.h"
#include "MainScene.h"

USING_NS_CC;
using namespace std;

Scene* MainScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = MainScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool MainScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    _winSize = Director::getInstance()->getWinSize();
    initBackground();
    initNumberNodes();
    return true;
}

void MainScene::initBackground() {
    // real bg
    _background = LayerColor::create(cfg::bgColor);
    addChild(_background);
    
    // square bg
    float wh = min(_winSize.width, _winSize.height) - cfg::squareBgMarginLeft * 2;
    
    _squareBackground = LayerColor::create(cfg::squareBgColor);
    _squareBackground->setContentSize(Size(wh,wh));
    // Please note that, the position of a layer is not relative to its anchor point!
    _squareBackground->setPosition(cfg::squareBgMarginLeft, (_winSize.height - wh)/2);
    _background->addChild(_squareBackground);
}

void MainScene::initNumberNodes() {    
}




