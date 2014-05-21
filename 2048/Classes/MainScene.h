#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cc.h"

class MainScene : public cc::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    // will create a scene and add MainScene as a child to it and return.
    static cc::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    // implement the "static create()" method manually
    // create() function will call init()
    CREATE_FUNC(MainScene);
    
    // step by step initialization
    void initBackground();
    
    void initNumberNodes();
    
private:
    cc::Size _winSize;
    // bg, has the same size with screen
    cc::LayerColor *_background;
    // the square contains number nodes.
    cc::LayerColor *_squareBackground;
};

#endif // __HELLOWORLD_SCENE_H__
