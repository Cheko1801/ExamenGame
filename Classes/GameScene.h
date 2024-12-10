#pragma once
#pragma once
#pragma once

#include "cocos2d.h"

enum DIR_DEF {
    UP = 1,
    DOWN,
    LEFT,
    RIGHT
};

class SnakeNode : public cocos2d::Ref {
public:
    int col, row;                // Columna y fila del nodo
    int dir;                      // Dirección en la que se mueve
    SnakeNode* Prenode;          // Nodo anterior
    cocos2d::Sprite* nodeSprite; // Sprite visual del nodo
};

class GameScene : public cocos2d::Layer {
public:
    
    static cocos2d::Scene* createscene();

    
    bool init();

    
    void update(float dt);

    
    virtual bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);

    
    void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);

    CREATE_FUNC(GameScene);

protected:
    SnakeNode* head;            // Cabeza de la serpiente
    SnakeNode* food;            // Comida de la serpiente
    cocos2d::Vector<SnakeNode*> body; // Cuerpo de la serpiente
    cocos2d::Sprite* background;  // Agregar puntero para el fondo
};
#pragma once
#pragma once
