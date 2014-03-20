
#include "SimpleAudioEngine.h"
#include "HelloWorldScene.h"

USING_NS_CC;
using namespace CocosDenshion;
using namespace std;

HelloWorld::~HelloWorld() {
    // release actions
    CC_SAFE_RELEASE(_growBomb);
    CC_SAFE_RELEASE(_rotateSprite);
    CC_SAFE_RELEASE(_shockwaveSequence);
    CC_SAFE_RELEASE(_swingHealth);
    CC_SAFE_RELEASE(_groundHit);
    CC_SAFE_RELEASE(_explosion);
}


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
    
    Director* dx = Director::getInstance();
    
    _screenSize = dx->getWinSize();
    
    _running = false;
    
    createGameScreen();
    
    createPools();
    
    createActions();
    
    _fallingObjects.reserve(40);

    EventListenerTouchAllAtOnce *listener = EventListenerTouchAllAtOnce::create();
    listener->onTouchesBegan = CC_CALLBACK_2(HelloWorld::onTouchesBegan, this);
    
    // 0 is forbidden for graph based priority
    getEventDispatcher()->addEventListenerWithFixedPriority(listener, 1);
    
    schedule(schedule_selector(HelloWorld::update));
    
    SimpleAudioEngine::getInstance()->playBackgroundMusic("background.mp3");
    
    return true;
}

void HelloWorld::update(float delta) {
    if(! _running) {
        return;
    }
    
    _meteorTimer += delta;
    if(_meteorTimer > _meteorInterval) {
        _meteorTimer = 0;
        resetMeteor();
    }
    
    
    _healthTimer += delta;
    if(_healthTimer > _healthInterval) {
        _healthTimer = 0;
        resetHealth();
    }
    
    _difficultyTimer += delta;
    if(_difficultyTimer > _difficultyInterval) {
        _difficultyTimer = 0;
        increaseDifficulty();
    }
    
}

void HelloWorld::onTouchesBegan(const vector<Touch*>& touches, Event* unused) {
    
}


void HelloWorld::fallingObjectDone(Node* sender) {
    
    // remove from array, FIXME: performance issue ?
    _fallingObjects.eraseObject(sender);
    sender->stopAllActions();
    sender->setRotation(0);
    
    // a meteor
    if(sender->getTag() == TAG_SPRITE_METEOR) {
        _energy -= 15;
        // show explosion animation
        sender->runAction((Action*)_groundHit->clone()->autorelease());
        // play explosion sound
        SimpleAudioEngine::getInstance()->playEffect("boom.wav");
        
    } else { // is a health drop
        sender->setVisible(false);
        
        if(_energy == 100) {
            _score += 25;
            char score[100] = { 0 };
            sprintf(score, "%i", _score);
            _scoreDisplay->setString(score);
        }else {
            _energy += 10;
            if(_energy > 100) {
                _energy = 100;
            }
        }
        SimpleAudioEngine::getInstance()->playEffect("health.wav");
    }
    
    if (_energy <= 0) {
        _energy = 0;
        stopGame();
        SimpleAudioEngine::getInstance()->playEffect("fire_truck.wav");
        // show game over
        _gameOverMessage->setVisible(true);
    }
    
    char value[100] = {0};
    sprintf(value, "%i%%", _energy);
    _energyDisplay->setString(value);
}


void HelloWorld::animationDone(Node* sender) {
    sender->setVisible(false);
}

// call back, hide
void HelloWorld::shockwaveDone() {
    _shockwave->setVisible(false);
}

// use a new meteor form the pool
void HelloWorld::resetMeteor() {
    
}

// use a new drop from the pool
void HelloWorld::resetHealth() {
    if(_fallingObjects.size() > 30) {
        return;
    }
    
    Sprite* health = _healthPool.at(_healthPoolIndex);
    _healthPoolIndex++;
    
    if(_healthPoolIndex == _healthPool.size()) {
        _healthPoolIndex = 0;
    }
    
    float health_x = rand() % int( _screenSize.width * .8) + _screenSize.width * .1;
    float health_target_x = rand() % int( _screenSize.width * .8) + _screenSize.width * .1;
    
    health->stopAllActions();
    health->cocos2d::Node::setPosition(health_x, _screenSize.height + health->getBoundingBox().size.height * .5);
    
    MoveTo* moveTo = MoveTo::create(_healthSpeed, Point(health_target_x, _screenSize.height * .15));
    CallFuncN* cb = CallFuncN::create( CC_CALLBACK_1(HelloWorld::fallingObjectDone, this));
    
    FiniteTimeAction* seq = Sequence::create(moveTo, cb, NULL);
    
    health->setVisible(true);
    health->runAction((Action*)_swingHealth->clone()->autorelease());
    health->runAction(seq);
    _fallingObjects.pushBack(health);
}

void HelloWorld::resetGame() {
    
}

void HelloWorld::increaseDifficulty() {
    _meteorInterval -= 0.2;
    if(_meteorInterval < .25) {
        _meteorInterval = 0.25;
    }
    
    _meteorSpeed -= 1;
    if(_meteorSpeed < 4 ) {
        _meteorSpeed = 4;
    }
    
    _healthSpeed -= 1;
    if(_meteorSpeed < 8) {
        _healthSpeed = 8;
    }
}

void HelloWorld::stopGame() {
    
}

void HelloWorld::createGameScreen() {
    Sprite* bg = Sprite::create("bg.png");
    bg->cocos2d::Node::setPosition(_screenSize.width * .5, _screenSize.height * .5);
    addChild(bg);
    
    // cache frames
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("sprite_sheet.plist");
    
    // prepare node to render
    _gameBatchNode = SpriteBatchNode::create("sprite_sheet.png");
    addChild(_gameBatchNode);
}

void HelloWorld::createPools() {
    
}

void HelloWorld::createActions() {
    
}









