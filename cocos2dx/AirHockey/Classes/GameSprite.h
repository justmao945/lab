//
//  GameSprite.h
//  Air Hockey
//
//  Created by Jianjun on 28/2/14.
//
//

#ifndef __Air_Hockey__GameSprite__
#define __Air_Hockey__GameSprite__

#include "cocos2d.h"

class GameSprite: public cocos2d::Sprite {
public:
    GameSprite();
    ~GameSprite();
    
    static GameSprite*  createWithFile(const char* filename);
    
    virtual void setPosition(const cocos2d::Point&);
    
    float radius() const;
    
    CC_SYNTHESIZE(cocos2d::Point, _nextPosition, NextPosition);
    CC_SYNTHESIZE(cocos2d::Point, _vector, Vector);
    CC_SYNTHESIZE(cocos2d::Touch*, _touch, Touch);
};


#endif /* defined(__Air_Hockey__GameSprite__) */
