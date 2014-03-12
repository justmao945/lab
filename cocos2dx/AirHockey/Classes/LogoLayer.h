//
//  LogoLayer.h
//  Air Hockey
//
//  Created by Jianjun on 2/3/14.
//
//

#ifndef __Air_Hockey__LogoLayer__
#define __Air_Hockey__LogoLayer__

#include "cocos2d.h"


class LogoLayer: public cocos2d::Layer {
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();
    
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    // implement the "static create()" method manually
    CREATE_FUNC(LogoLayer);
    
    
    virtual bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event);
    
};


#endif /* defined(__Air_Hockey__LogoLayer__) */
