#include "GameLayer.h"

using namespace std;
USING_NS_CC;

GameLayer::GameLayer() {
    
}

GameLayer::~GameLayer() {
    CC_SAFE_RELEASE(_player1);
    CC_SAFE_RELEASE(_player2);
}

Scene* GameLayer::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = GameLayer::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool GameLayer::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    Director* director = Director::getInstance();
    
    _score1 = _score2 = 0;
    _screenSize = director->getWinSize();
    
    Sprite *court = Sprite::create("court.png");
    court->setPosition( Point(_screenSize.width * .5, _screenSize.height * .5) );
    addChild(court);
    
    _player1 = GameSprite::createWithFile("mallet.png");
    _player1->setPosition(Point(_screenSize.width * .5, _player1->radius() * 2));
    addChild(_player1);
    
    _player2 = GameSprite::createWithFile("mallet.png");
    _player2->setPosition(Point(_screenSize.width * .5, _screenSize.height - _player2->radius() * 2));
    addChild(_player2);
    
    _player1->retain();
    _player2->retain();
    
    _ball = GameSprite::createWithFile("puck.png");
    _ball->setPosition(Point(_screenSize.width * .5, _screenSize.height * .5 - _ball->radius() * 2));
    addChild(_ball);
    
    _score1Label = LabelTTF::create("0", "Arial", 60);
    _score1Label->setPosition(Point(_screenSize.width - 60, _screenSize.height * .5 - 80));
    _score1Label->setRotation(90);
    addChild(_score1Label);
    
    _score2Label = LabelTTF::create("0", "Arial", 60);
    _score2Label->setPosition(Point(_screenSize.width - 60, _screenSize.height * .5 + 80));
    _score2Label->setRotation(90);
    addChild(_score2Label);
    
    EventListenerTouchOneByOne *listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = CC_CALLBACK_2(GameLayer::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(GameLayer::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(GameLayer::onTouchEnded, this);
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, _player1);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener->clone(), _player2);
    
    schedule(schedule_selector(GameLayer::update));
    
    return true;
}

void GameLayer::onTouchesBegan(const vector<Touch*>& touches, Event *unused_event) {
    
}

void GameLayer::onTouchesMoved(const vector<Touch*>& touches, Event *unused_event) {
    
}

void GameLayer::onTouchesEnded(const vector<Touch*>& touches, Event *unused_event) {
    
}

void GameLayer::update(float dt) {
    
}

