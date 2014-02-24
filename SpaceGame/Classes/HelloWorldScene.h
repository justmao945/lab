#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

class HelloWorld : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
    
private:
    // update schedule
    void update(float dt);
    
private:
    cocos2d::Sprite *mShip;
    cocos2d::SpriteBatchNode *mBatchNode;
    cocos2d::ParallaxNode *mBackgroundNode;
    cocos2d::Sprite *mSpaceDust1, *mSpaceDust2;
    cocos2d::Sprite *mPlanetSunrise;
    cocos2d::Sprite *mGalaxy;
    cocos2d::Sprite *mSpacialAnomaly, *mSpacialAnomaly2;
};

#endif // __HELLOWORLD_SCENE_H__
