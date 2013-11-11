#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"

using namespace cocos2d;
using namespace CocosDenshion;

/**
 * シーン設定
 * @return scene
 */
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

/**
 * init
 * @return {Bool}
 */

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
    
    // マルチタッチモード
    this->setTouchMode(kCCTouchesAllAtOnce);
    
    // タッチを有効にする
    this->setTouchEnabled(true);
    
    // 毎フレームでupdate()を実行
    this->schedule(schedule_selector(HelloWorld::update));
    
    // 衝突判定
    this->scheduleUpdate();

    // 終了ボタン
    CCMenuItemImage* pCloseItem = CCMenuItemImage::create("button_close.png", "button_close_pressed.png", this, menu_selector(HelloWorld::menuCloseCallback));
    
    // 表示位置を判定
    pCloseItem->setPosition(ccp(winSize.width - pCloseItem->getContentSize().width / 2, 
                                pCloseItem->getContentSize().height / 2));
    
    // CCMenuItemImageからCCMenuを作成
    CCMenu* pMenu = CCMenu::create(pCloseItem, NULL);
    
    // CCMenuの位置指定
    pMenu->setPosition(CCPointZero);
    
    // CCMenuをレイヤについか
    this->addChild(pMenu);
    
    // トグルボタン
    // Offボタン通常時
    CCSprite* itemOff1 = CCSprite::create("button_check_off.png");
    // Offのタップ時は色を変える
    CCSprite* itemOff2 = CCSprite::create("button_check_off.png");
    itemOff2->setColor(ccc3(102, 102, 102));
    
    CCMenuItemSprite* menuItemOff = CCMenuItemSprite::create(itemOff1, itemOff2, NULL);
    
    // Onボタン通常時
    CCSprite* itemOn1 = CCSprite::create("button_check_on.png");
    // Onボタン時は色を変える
    CCSprite* itemOn2 = CCSprite::create("button_check_on.png");
    itemOn2->setColor(ccc3(102, 102, 102));
    
    CCMenuItemSprite* menuItemOn = CCMenuItemSprite::create(itemOn1, itemOn2, NULL);
    
    // OnとOffのCCMenuItemとコールバックを指定してCCMenuItemToggleを作成
    CCMenuItemToggle* toggleItem = CCMenuItemToggle::createWithTarget(this,
                                                                      menu_selector(HelloWorld::changeMode),
                                                                      menuItemOff,
                                                                      menuItemOn,
                                                                      NULL);
    // 表示位置を指定
    toggleItem->setPosition(ccp(pCloseItem->getContentSize().width/2,
                                winSize.height - pCloseItem->getContentSize().height/2));
    // CCMenuItemToggleからCCMenuを作成
    CCMenu* toggleMenu = CCMenu::create(toggleItem, NULL);
    toggleMenu->setPosition(CCPointZero);
    toggleMenu->setTag(3);
    
    // CMenuをレイヤに追加
    this->addChild(toggleMenu);
    
    // 

    return true;
}

/**
 * 食べ物を追加
 * @return {void}
 */
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
    
    // スプライト非表示対応
    CCCallFuncN* actionMoveDone = CCCallFuncN::create(this, callfuncN_selector(HelloWorld::spriteMoveFinished));

    // アクション実行
    food->runAction(CCSequence::create(actionMove, actionMoveDone, NULL));
}

/**
 * ゲームロジック生成
 * @return void
 */
void HelloWorld::gameLogic()
{
    this->addFood();
}

/**
 * 終了したスプライトを非表示にする
 * @param  {CCNode} スプライト
 * @return {void}
 */

void HelloWorld::spriteMoveFinished(cocos2d::CCNode *sender)
{
    // 終了したアクションのスプライトを取得
    CCSprite* sprite = (CCSprite*) sender;

    // spriteを削除
    bool isCleanUp = false;
    
    // スプライトをレイヤから削除
    this->removeChild(sprite, isCleanUp);
}

/**
 * タッチイベントを登録
 * @param  {CCSet}   タッチ
 * @param  {CCEvent} イベント
 * @return {void}
 */
void HelloWorld::ccTouchesBegan(cocos2d::CCSet *touches, cocos2d::CCEvent *event)
{
    // タッチされた座標を取得
    CCTouch* touch = (CCTouch*)touches->anyObject();
    CCPoint location = touch->getLocationInView();
    location = CCDirector::sharedDirector()->convertToGL(location);
    
    // ウッホイ君を取得(tag=1で識別）
    CCSprite* player = (CCSprite*)this->getChildByTag(1);
    
    // ウッホイ君をタッチされた座標まで移動
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    
    // 継続時間は移動距離に比例させる
    float length =sqrtf(powf(location.x - player->getPositionX(), 2.0f) + powf(location.y - player->getPositionY(), 2.0f));
    
    float duration = length / winSize.width * 0.5f;
    CCMoveTo* actionMove = CCMoveTo::create(duration, location);
    
    player->runAction(actionMove);
    
}

/**
 * 衝突判定
 * @param  {Float}  dt
 * @return {void}
 */
void HelloWorld::update(float dt)
{
    // ウッホイ君のスプライトを取得(tag=1で識別)
    CCSprite* player = (CCSprite*)this->getChildByTag(1);
    // 判定基準の領域
    CCRect playerRect = CCRectMake(player->getPosition().x - (player->getContentSize().width/2), 
                                   player->getPosition().y - (player->getContentSize().height/2), 
                                                              player->getContentSize().width/2, 
                                                              player->getContentSize().height/2);

    // ハンバーガーのスプライトを取得(tag=2で識別)
    CCSprite* food = (CCSprite*)this->getChildByTag(2);
    // ハンバーガーがなければ処理を抜ける
    if (food==NULL) {
        return;
    }
    // 判定基準の領域
    CCRect foodRect = CCRectMake(food->getPosition().x - (food->getContentSize().width/2), 
                                 food->getPosition().y - (food->getContentSize().height/2), 
                                 food->getContentSize().width, 
                                 food->getContentSize().height);

    // 衝突判定
    if (playerRect.intersectsRect(foodRect)) {
        // ハンバーガーを削除
        this->removeChild(food, true);
        
        // ウッホイ君のイメージ変更
        player->setTexture(CCTextureCache::sharedTextureCache()->addImage("monkey02.png"));
        
        // 0.1秒後にHelloWorld::eat()を１度だけ実行
        this->scheduleOnce(schedule_selector(HelloWorld::eat), 0.1f);
    }

}

/**
 * 食べる実行
 * @return {void}
 */
void HelloWorld::eat()
{
    // ウッホイ君の画像を元に戻す
    CCSprite* player = (CCSprite*)this->getChildByTag(1);
    
    player->setTexture(CCTextureCache::sharedTextureCache()->addImage("monkey01.png"));
}

/**
 * ボタンタップのコールバック
 * @param  {[type]}
 * @return {void}
 */
void HelloWorld::menuCloseCallback(CCObject* pSender)
{
    // アプリを終了する
    CCDirector::sharedDirector()->end();
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

/**
 * トグルボタン　タップ
 * @param  {[type]}
 * @return {void}
 */
void HelloWorld::changeMode(CCObject* sender)
{
    // トグルボタン内で選択中のアイテムのIndexを取得
    int selectIndex = dynamic_cast<CCMenuItemToggle*>(sender)->getSelectedIndex();
    
    CCSprite* player = (CCSprite *)this->getChildByTag(1);
    // トグルボタンが0nの時にウッホイ君を非表示にして、OFFの時は表示する
    player->setVisible(selectIndex == 0);
    
}











