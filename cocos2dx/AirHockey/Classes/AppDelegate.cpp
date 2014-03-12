#include "SimpleAudioEngine.h"
#include "AppDelegate.h"
#include "GameLayer.h"
#include "LogoLayer.h"

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
    
    glview->setDesignResolutionSize(768, 1024, ResolutionPolicy::EXACT_FIT);

    FileUtils* futils = FileUtils::getInstance();
    
    auto screensize = glview->getFrameSize();
    if(screensize.width > 768) {
        vector<string> dirs(1, "hd");
        futils->setSearchResolutionsOrder(dirs);
        director->setContentScaleFactor(2);
    }
    else {
        vector<string> dirs(1, "sd");
        futils->setSearchResolutionsOrder(dirs);
        director->setContentScaleFactor(1);
    }
    
    SimpleAudioEngine* augine = SimpleAudioEngine::getInstance();
    
    augine->preloadEffect(futils->fullPathForFilename("hit.wav").c_str());
    augine->preloadEffect(futils->fullPathForFilename("score.wav").c_str());
    
    // turn on display FPS
    director->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);

    // create a scene. it's an autorelease object
    auto scene = LogoLayer::createScene();

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
