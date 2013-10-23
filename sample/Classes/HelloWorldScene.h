#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"


class HelloWorld : public cocos2d::CCLayer
{
public:
    // Method 'init' in cocos2d-x returns bool, instead of 'id' in cocos2d-iphone (an object pointer)
    virtual bool init();

    // there's no 'id' in cpp, so we recommend to return the class instance pointer
    static cocos2d::CCScene* scene();
    
    void addFood();
    
    void gameLogic();
    
    // 宣言追加
    void spriteMoveFinished(CCNode* sender);

    // タッチイベント追加
    void ccTouchesBegan(cocos2d::CCSet* touches, cocos2d::CCEvent* event);
    
    // 衝突判定
    void update(float dt);
    
    // 食べる実行
    void eat();
    
    // ImageのCallback用
    void menuCloseCallback(CCObject* pSender);
    
    
    // preprocessor macro for "static create()" constructor ( node() deprecated )
    CREATE_FUNC(HelloWorld);
};

#endif // __HELLOWORLD_SCENE_H__
        