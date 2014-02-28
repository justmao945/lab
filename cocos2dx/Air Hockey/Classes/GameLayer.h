#ifndef __GAME_LAYER_H__
#define __GAME_LAYER_H__

#include "cocos2d.h"
#include "GameSprite.h"

#define GOAL_WIDTH 400

class GameLayer : public cocos2d::Layer
{
public:
    GameLayer();
    ~GameLayer();
    
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  
    
    // implement the "static create()" method manually
    CREATE_FUNC(GameLayer);
    
    void setPlayerScore(int player);
    
    virtual void onTouchesBegan(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event *unused_event);
    virtual void onTouchesMoved(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event *unused_event);
    virtual void onTouchesEnded(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event *unused_event);

    void update(float dt);
    
protected:
    GameSprite* _player1;
    GameSprite* _player2;
    GameSprite* _ball;
    
    cocos2d::LabelTTF *_score1Label;
    cocos2d::LabelTTF *_score2Label;
    
    cocos2d::Size _screenSize;
    
    int _score1;
    int _score2;
};

#endif // __GAME_LAYER_H__
