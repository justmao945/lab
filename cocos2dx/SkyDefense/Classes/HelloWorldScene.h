#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

enum SpriteTags {
    TAG_SPRITE_BOMB,
    TAG_SPRITE_SHOCKWAVE,
    TAG_SPRITE_METEOR,
    TAG_SPRITE_HEALTH,
    TAG_SPRITE_HALO,
    TAG_SPRITE_SPARKLE,
    
    TAG_BACKGROUND,
    TAG_MIDDLE_GROUND,
    TAG_FOREGROUND,
};


class HelloWorld : public cocos2d::Layer
{
public:
    
    ~HelloWorld();
    
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  
    
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
    
    void fallingObjectDone(cocos2d::Node* sender);
    
    void animationDone(cocos2d::Node* sender);
    
    void shockwaveDone();
    
    virtual void onTouchesBegan(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event *unused_event);
    
    virtual void update(float delta);
    
private:
    void resetMeteor();
    
    void resetHealth();
    
    void resetGame();
    
    void stopGame();
    
    void increaseDifficulty();
    
    void createGameScreen();
    
    void createPools();
    
    void createActions();
    
    
private:
    
    cocos2d::Vector<cocos2d::Sprite*> _meteorPool;
    int _meteorPoolIndex;
    
    cocos2d::Vector<cocos2d::Sprite*> _healthPool;
    int _healthPoolIndex;
    
    cocos2d::Vector<cocos2d::Node*> _fallingObjects;
    int _clouds;
    
    cocos2d::SpriteBatchNode *_gameBatchNode;
    cocos2d::Sprite* _bomb;
    cocos2d::Sprite* _shockwave;
    
    cocos2d::Sprite* _introMessage;
    cocos2d::Sprite* _gameOverMessage;
    
    cocos2d::LabelBMFont* _energyDisplay;
    cocos2d::LabelBMFont* _scoreDisplay;
    
    cocos2d::Action* _growBomb;
    cocos2d::Action* _rotateSprite;
    cocos2d::Action* _shockwaveSequence;
    cocos2d::Action* _swingHealth;
    
    cocos2d::Action* _groundHit;
    cocos2d::Action* _explosion;
    
    cocos2d::Size _screenSize;
    
    float _meteorInterval;
    float _meteorTimer;
    float _meteorSpeed;
    
    float _healthInterval;
    float _healthTimer;
    float _healthSpeed;
    
    float _difficultyInterval;
    float _difficultyTimer;
    
    int _energy;
    int _score;
    int _shockwaveHits;
    bool _running;
};

#endif // __HELLOWORLD_SCENE_H__
