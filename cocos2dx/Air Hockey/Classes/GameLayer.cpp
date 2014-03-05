#include "SimpleAudioEngine.h"
#include "GameLayer.h"

using namespace std;
using namespace CocosDenshion;
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
    _players.pushBack(_player1);
    
    _player2 = GameSprite::createWithFile("mallet.png");
    _player2->setPosition(Point(_screenSize.width * .5, _screenSize.height - _player2->radius() * 2));
    addChild(_player2);
    _players.pushBack(_player2);
    
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
    
    EventListenerTouchAllAtOnce *listener = EventListenerTouchAllAtOnce::create();
    listener->onTouchesBegan = CC_CALLBACK_2(GameLayer::onTouchesBegan, this);
    listener->onTouchesMoved = CC_CALLBACK_2(GameLayer::onTouchesMoved, this);
    listener->onTouchesEnded = CC_CALLBACK_2(GameLayer::onTouchesEnded, this);
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, _player1);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener->clone(), _player2);
    
    schedule(schedule_selector(GameLayer::update));
    
    return true;
}

void GameLayer::onTouchesBegan(const vector<Touch*>& touches, Event *unused_event) {
    for(auto touch: touches) {
        if(! touch) continue;
        for (auto player: _players) {
            if(player->getBoundingBox().containsPoint(touch->getLocation())) {
                player->setTouch(touch);
            }
        }
    }
}

void GameLayer::onTouchesMoved(const vector<Touch*>& touches, Event *unused_event) {
    for(auto touch: touches) {
        if(! touch) continue;
        for (auto player: _players) {
            if(player->getTouch() && player->getTouch() == touch) {
                Point nextPos = touch->getLocation();
                
                if(nextPos.x < player->radius()) {
                    nextPos.x = player->radius();
                }
                else if(nextPos.x > _screenSize.width - player->radius()) {
                    nextPos.x = _screenSize.width - player->radius();
                }
                
                if(nextPos.y < player->radius()) {
                    nextPos.y = player->radius();
                }
                else if( nextPos.y > _screenSize.height - player->radius()) {
                    nextPos.y = _screenSize.height - player->radius();
                }
                
                if(player->getPositionY() < _screenSize.height * .5) {
                    if(nextPos.y > _screenSize.height * .5 - player->radius()) {
                        nextPos.y = _screenSize.height * .5 - player->radius();
                    }
                }
                else {
                    if(nextPos.y < _screenSize.height * .5 + player->radius()) {
                        nextPos.y = _screenSize.height * .5 + player->radius();
                    }
                }
                
                player->setNextPosition(nextPos);
                player->setVector(touch->getLocation() - player->getPosition());
            }
        }
    }
}

void GameLayer::onTouchesEnded(const vector<Touch*>& touches, Event *unused_event) {
    for(auto touch: touches) {
        if(! touch) continue;
        for (auto player: _players) {
            if(player->getTouch() && player->getTouch() == touch) {
                player->setTouch(nullptr);
                player->setVector(Point(0,0));
            }
        }
    }
}

void GameLayer::update(float dt) {
    Point ballVector = _ball->getVector() * .98;
    Point ballNextPos = _ball->getNextPosition() + ballVector;
    float distii = _player1->radius() * _player1->radius() + _ball->radius() * _ball->radius();

    for(auto player: _players) {
        float dist1ii = player->getPosition().getDistanceSq(ballNextPos);
        float dist2ii = player->getNextPosition().getDistanceSq(_ball->getPosition());
        if(dist1ii <= distii || dist2ii < distii) {
            float mag_ball = _ball->getVector().getLengthSq();
            float mag_player = player->getVector().getLengthSq();
            float force = sqrt(mag_ball + mag_player);
            float angle = atan2(ballNextPos.y - player->getPositionY(), ballNextPos.x - player->getPositionX());
            ballVector.x = force * cos(angle);
            ballVector.y = force * sin(angle);
            ballNextPos.x = player->getNextPosition().x + (player->radius() + _ball->radius() + force) * cos(angle);
            ballNextPos.y = player->getNextPosition().y + (player->radius() + _ball->radius() + force) * sin(angle);
            SimpleAudioEngine::getInstance()->playEffect("hit.wav");
        }
    }
    if (ballNextPos.x < _ball->radius()) {
        ballNextPos.x = _ball->radius();
        ballVector.x *= -0.8f;
        SimpleAudioEngine::getInstance()->playEffect("hit.wav");
    }
    if (ballNextPos.x > _screenSize.width - _ball->radius()) {
        ballNextPos.x = _screenSize.width - _ball->radius();
        ballVector.x *= -0.8f;
        SimpleAudioEngine::getInstance()->playEffect("hit.wav");
    }
    if (ballNextPos.y > _screenSize.height - _ball->radius()) {
        if (_ball->getPosition().x < _screenSize.width * 0.5f - GOAL_WIDTH * 0.5f
                || _ball->getPosition().x > _screenSize.width * 0.5f + GOAL_WIDTH * 0.5f) {
            ballNextPos.y = _screenSize.height - _ball->radius();
            ballVector.y *= -0.8f;
            SimpleAudioEngine::getInstance()->playEffect("hit.wav");
        }
    }
    if (ballNextPos.y < _ball->radius() ) {
        if (_ball->getPosition().x < _screenSize.width * 0.5f - GOAL_WIDTH * 0.5f
            || _ball->getPosition().x > _screenSize.width * 0.5f + GOAL_WIDTH * 0.5f) {
            ballNextPos.y = _ball->radius();
            ballVector.y *= -0.8f;
            SimpleAudioEngine::getInstance()->playEffect("hit.wav");
        }
    }
    _ball->setVector(ballVector);
    _ball->setNextPosition(ballNextPos);
    //check for goals!
    if (ballNextPos.y  < -_ball->radius() * 2) {
        setPlayerScore(2);
    }
    if (ballNextPos.y > _screenSize.height + _ball->radius() * 2)
    {
        setPlayerScore(1);
    }
    _player1->setPosition(_player1->getNextPosition());
    _player2->setPosition(_player2->getNextPosition());
    _ball->setPosition(_ball->getNextPosition());
}

void GameLayer::setPlayerScore(int player) {
    SimpleAudioEngine::getInstance()->playEffect("score.wav");
    _ball->setVector(Point(0,0));
    char score_buffer[20];
    if (player == 1) {
        _score1++;
        sprintf(score_buffer,"%i", _score1);
        _score1Label->setString(score_buffer);
        _ball->setNextPosition(Point(_screenSize.width * 0.5, _screenSize.height * 0.5 + 2 * _ball->radius()));
    } else {
        _score2++;
        sprintf(score_buffer,"%i", _score2);
        _score2Label->setString(score_buffer);
        _ball->setNextPosition(Point(_screenSize.width * 0.5, _screenSize.height * 0.5 - 2 * _ball->radius()));
    }
    _player1->setPosition(Point(_screenSize.width * 0.5, _player1->radius() * 2));
    _player2->setPosition(Point(_screenSize.width * 0.5, _screenSize.height - _player1->radius() * 2));
    _player1->setTouch(nullptr);
    _player2->setTouch(nullptr);
}


