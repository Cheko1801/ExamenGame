
#include "HelloWorldScene.h"
#include "ui\CocosGUI.h"
#include "cocos2d.h"
#include "GameScene.h"
#include "AudioEngine.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();

    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if (!Layer::init())
    {
        return false;
    }
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    auto sprite = Sprite::create("bck.png");

    // position the sprite on the center of the screen
    sprite->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
    if (!sprite)
    {
        CCLOG("Error: No se pudo cargar el fondo de la escena.");
        return false;  // Detener la inicialización si no se puede cargar la imagen
    }
    // add the sprite as a child to this layer
    this->addChild(sprite, 0);

    auto start_button = ui::Button::create("Button_Start.png");
    start_button->setPosition(Point(visibleSize.width / 2, visibleSize.height / 2 - 100));
    start_button->addTouchEventListener([](Ref* pSender, ui::Widget::TouchEventType type) {
        if (type == ui::Widget::TouchEventType::ENDED) {
            auto transition = TransitionFade::create(0.5, GameScene::createscene());
            Director::getInstance()->replaceScene(transition);
        }
        });
    this->addChild(start_button);

    return true;
}


void HelloWorld::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif

    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() and exit(0) as given above,instead trigger a custom event created in RootViewController.mm as below*/

    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);


}
