#include <vector>
#include "SimpleAudioEngine.h"
#include "AppDelegate.h"
#include "HelloWorldScene.h"

USING_NS_CC;
using namespace std;
using namespace CocosDenshion;

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
        glview = GLView::create("My Game");
        director->setOpenGLView(glview);
    }

    // get win size, real screen size, hardware resolution
    Size framesize = glview->getFrameSize();
    
    // the size be used to design the game, do all works in this resolution
    Size designsize = Size(2048, 1536);
    
    glview->setDesignResolutionSize(designsize.width, designsize.height, ResolutionPolicy::EXACT_FIT);
    
    FileUtils* futil = FileUtils::getInstance();

    if(framesize.height > 768) {
        futil->addSearchPath("ipadhd");
    }
    else if(framesize.height > 320) {
        futil->addSearchPath("ipad");
    }
    else {
        futil->addSearchPath("iphone");
    }
    
    // working resolution is designsize, and real hardware resolution is frame.
    // resource resolution = scale factor * design reslution
    // here resource resolution and screen resolution are set to be equal...
    director->setContentScaleFactor(framesize.height / designsize.height);
    
    // preload all audio resources.
    SimpleAudioEngine *sae = SimpleAudioEngine::getInstance();
    sae->preloadBackgroundMusic("background.mp3");
    sae->preloadEffect("bombFail.wav");
    sae->preloadEffect("bombRelease.wav");
    sae->preloadEffect("bomb.wav");
    sae->preloadEffect("fire_truck.wav");
    sae->preloadEffect("health.wav");
    
    sae->setEffectsVolume(.4f);
    
    // turn on display FPS
    director->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);

    // create a scene. it's an autorelease object
    auto scene = HelloWorld::createScene();

    // run
    director->runWithScene(scene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
}
