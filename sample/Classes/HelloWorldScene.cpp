#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"

using namespace cocos2d;
using namespace CocosDenshion;

CCScene* HelloWorld::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    HelloWorld *layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

bool HelloWorld::init()
{
    if ( !CCLayer::init() )
    {
        return false;
    }

    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    
    CCSprite* player = CCSprite::create("monkey01.png", CCRectMake(0, 0, 100, 135));
    
    player->setPosition(ccp(player->getContentSize().width*3/2, winSize.height/2));
    
    player->setTag(1);
    
    this->addChild(player);
        
    this->schedule(schedule_selector(HelloWorld::gameLogic), 3.0);
    
    return true;
}

void HelloWorld::addFood()
{
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    
    CCSprite* food = CCSprite::create("hamburger.png", CCRectMake(0, 0, 36, 30));
    
    int height = rand() % (int)winSize.height;
    
    food->setPosition(ccp(winSize.width + food->getContentSize().width/2, height));
    
    food->setTag(2);
    
    this->addChild(food);
    
    float duration = 2.0f;
    
    CCMoveTo* actionMove = CCMoveTo::create(duration, ccp(food->getContentSize().width*3/2, food->getPositionY()));
    
    food->runAction(actionMove);
}

void HelloWorld::gameLogic()
{
    this->addFood();
}