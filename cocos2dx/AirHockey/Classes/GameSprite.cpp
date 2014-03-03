//
//  GameSprite.cpp
//  Air Hockey
//
//  Created by Jianjun on 28/2/14.
//
//

#include <cassert>
#include "GameSprite.h"

using namespace cocos2d;

GameSprite::GameSprite() : _vector(0,0) {
    
}

GameSprite::~GameSprite() {
    
}

GameSprite* GameSprite::createWithFile(const char* filename) {
    assert(filename);
    GameSprite *sprite = new GameSprite();
    if(sprite && sprite->initWithFile(filename)) {
        sprite->autorelease();
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return nullptr;
}

void GameSprite::setPosition(const Point& pos) {
    Sprite::setPosition(pos);
    if(_nextPosition != pos) {
        _nextPosition = pos;
    }
}

float GameSprite::radius() const {
    return getTexture()->getContentSize().width * .5f;
}