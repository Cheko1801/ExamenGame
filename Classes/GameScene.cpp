#include "GameScene.h"
#include "cocos2d.h"
#include "ui\CocosGUI.h"
#include "HelloWorldScene.h"





USING_NS_CC;

Scene* GameScene::createscene() {
    auto scene = Scene::create();
    auto layer = GameScene::create();

    scene->addChild(layer);

    return scene;
}

bool GameScene::init() {
    if (!Layer::init()) {
        return false;
    }
    Size visibleSize = Director::getInstance()->getVisibleSize();


    // Ajusta las dimensiones de la cancha (20x15)
    int numColumns = 10;
    int numRows = 8;
    background = cocos2d::Sprite::create("backgroundd.png"); // Asegúrate de tener la imagen "background.png" en tu carpeta de recursos
    background->setPosition(cocos2d::Vec2(320, 240)); // Centrar el fondo (ajusta si es necesario)
    this->addChild(background, 0); // Agregar el fondo a la capa (0 significa que está detrás de otros elementos)


    head = new SnakeNode();
    head->col = rand() % numColumns; // Ajustar dentro de 20 columnas
    head->row = rand() % numRows;    // Ajustar dentro de 15 filas

    food = new SnakeNode();
    food->col = rand() % numColumns;
    food->row = rand() % numRows;

    head->nodeSprite = Sprite::create("head_d.png");
    head->nodeSprite->setAnchorPoint(Point::ANCHOR_BOTTOM_LEFT);
    head->nodeSprite->setPosition(Vec2(head->col * 32, head->row * 32));
    this->addChild(head->nodeSprite, 0);

    food->nodeSprite = Sprite::create("apple.png");
    food->nodeSprite->setAnchorPoint(Point::ANCHOR_BOTTOM_LEFT);
    food->nodeSprite->setPosition(Vec2(food->col * 32, food->row * 32));
    this->addChild(food->nodeSprite, 0);

    // Definir la velocidad inicial
    float initialSpeed = 0.2f;
    this->schedule([this](float delta) {
        this->update(delta);
        }, initialSpeed, "update_key");

    auto touchlistener = EventListenerTouchOneByOne::create();
    touchlistener->setSwallowTouches(true);
    touchlistener->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBegan, this);

    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchlistener, this);

    // Agregar la captura de teclas
    auto keyboardListener = EventListenerKeyboard::create();
    keyboardListener->onKeyPressed = CC_CALLBACK_2(GameScene::onKeyPressed, this);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(keyboardListener, this);

    return true;
}


bool GameScene::onTouchBegan(Touch* touch, Event* event) {
    Point touchPoint = touch->getLocation();

    int touchCol = (int)touchPoint.x / 32;
    int touchRow = (int)touchPoint.y / 32;

    if (abs(touchCol - head->col) > abs(touchRow - head->row)) {
        if (touchCol > head->col) {
            if (head->dir != RIGHT && head->dir != LEFT)
                head->dir = DIR_DEF::RIGHT;
        }
        else {
            if (head->dir != RIGHT && head->dir != LEFT)
                head->dir = DIR_DEF::LEFT;
        }
    }
    else {
        if (touchRow > head->row) {
            if (head->dir != UP && head->dir != DOWN)
                head->dir = DIR_DEF::UP;
        }
        else {
            if (head->dir != UP && head->dir != DOWN)
                head->dir = DIR_DEF::DOWN;
        }
    }

    return true;
}

void GameScene::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event) {
    // Cambiar dirección de la serpiente dependiendo de la tecla presionada
    if (keyCode == EventKeyboard::KeyCode::KEY_W) {
        if (head->dir != DIR_DEF::DOWN) { // No puede ir en dirección opuesta
            head->dir = DIR_DEF::UP;
        }
    }
    else if (keyCode == EventKeyboard::KeyCode::KEY_S) {
        if (head->dir != DIR_DEF::UP) {
            head->dir = DIR_DEF::DOWN;
        }
    }
    else if (keyCode == EventKeyboard::KeyCode::KEY_A) {
        if (head->dir != DIR_DEF::RIGHT) {
            head->dir = DIR_DEF::LEFT;
        }
    }
    else if (keyCode == EventKeyboard::KeyCode::KEY_D) {
        if (head->dir != DIR_DEF::LEFT) {
            head->dir = DIR_DEF::RIGHT;
        }
    }
}

void GameScene::update(float dt) {
    // Actualizar el movimiento del cuerpo del gusano
    for (int i = body.size() - 1; i >= 0; i--) {
        auto sn = body.at(i);
        if (body.empty()) {
            sn->col = head->col;
            sn->row = head->row;
            sn->dir = head->dir;
        }
        else {
            sn->col = sn->Prenode->col;
            sn->row = sn->Prenode->row;
            sn->dir = sn->Prenode->dir;
        }
        sn->nodeSprite->setPosition(Point(sn->col * 32, sn->row * 32));
    }

    // Mover la cabeza dependiendo de la dirección
    switch (head->dir) {
    case DIR_DEF::UP:
        head->row++;
        head->nodeSprite->setAnchorPoint(Point::ANCHOR_TOP_RIGHT);
        head->nodeSprite->setRotation(-180);
        // Si sale por arriba, que aparezca por abajo
        if (head->row >= 18) {
            head->nodeSprite->setVisible(false);
            for (auto sn : body) {
                sn->nodeSprite->setVisible(false);
            }
            food->nodeSprite->setVisible(false);
            Label* scoreLabel = Label::createWithTTF("Puntaje: " + std::to_string(body.size()), "fonts/Marker Felt.ttf", 24);
            scoreLabel->setPosition(Vec2(100, 100));
            this->addChild(scoreLabel);
            // Crear el botón Game Over
            Size visibleSize = Director::getInstance()->getVisibleSize();
            auto gameOverButton = ui::Button::create("background.png");
            gameOverButton->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));

            // Configurar la acción del botón para regresar al menú principal (MainMenu)
            gameOverButton->addTouchEventListener([=](Ref* pSender, ui::Widget::TouchEventType type) {
                if (type == ui::Widget::TouchEventType::ENDED) {
                    // Crear una transición hacia la escena HelloWorld (MainMenu)
                    auto scene = HelloWorld::createScene();
                    auto transition = TransitionFade::create(1.0f, scene);
                    Director::getInstance()->replaceScene(transition);
                }
                });

            // Añadir el botón Game Over a la escena
            this->addChild(gameOverButton);
        }
        break;
    case DIR_DEF::DOWN:
        head->row--;
        head->nodeSprite->setAnchorPoint(Point::ANCHOR_BOTTOM_LEFT);
        head->nodeSprite->setRotation(0);
        // Si sale por abajo, que aparezca por arriba
        if (head->row < 0) {
            head->nodeSprite->setVisible(false);
            for (auto sn : body) {
                sn->nodeSprite->setVisible(false);
            }
            food->nodeSprite->setVisible(false);
            Label* scoreLabel = Label::createWithTTF("Puntaje: " + std::to_string(body.size()), "fonts/Marker Felt.ttf", 24);
            scoreLabel->setPosition(Vec2(100, 100));
            this->addChild(scoreLabel);
            // Crear el botón Game Over
            Size visibleSize = Director::getInstance()->getVisibleSize();
            auto gameOverButton = ui::Button::create("background.png");
            gameOverButton->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));

            // Configurar la acción del botón para regresar al menú principal (MainMenu)
            gameOverButton->addTouchEventListener([=](Ref* pSender, ui::Widget::TouchEventType type) {
                if (type == ui::Widget::TouchEventType::ENDED) {
                    // Crear una transición hacia la escena HelloWorld (MainMenu)
                    auto scene = HelloWorld::createScene();
                    auto transition = TransitionFade::create(1.0f, scene);
                    Director::getInstance()->replaceScene(transition);
                }
                });

            // Añadir el botón Game Over a la escena
            this->addChild(gameOverButton);
        }
        break;
    case DIR_DEF::LEFT:
        head->col--;
        head->nodeSprite->setAnchorPoint(Point::ANCHOR_BOTTOM_RIGHT);
        head->nodeSprite->setRotation(90);
        // Si sale por la izquierda, que aparezca por la derecha
        if (head->col < 0) {
            head->nodeSprite->setVisible(false);
            for (auto sn : body) {
                sn->nodeSprite->setVisible(false);
            }
            food->nodeSprite->setVisible(false);
            Label* scoreLabel = Label::createWithTTF("Puntaje: " + std::to_string(body.size()), "fonts/Marker Felt.ttf", 24);
            scoreLabel->setPosition(Vec2(100, 100));
            this->addChild(scoreLabel);
            // Crear el botón Game Over
            Size visibleSize = Director::getInstance()->getVisibleSize();
            auto gameOverButton = ui::Button::create("background.png");
            gameOverButton->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));

            // Configurar la acción del botón para regresar al menú principal (MainMenu)
            gameOverButton->addTouchEventListener([=](Ref* pSender, ui::Widget::TouchEventType type) {
                if (type == ui::Widget::TouchEventType::ENDED) {
                    // Crear una transición hacia la escena HelloWorld (MainMenu)
                    auto scene = HelloWorld::createScene();
                    auto transition = TransitionFade::create(1.0f, scene);
                    Director::getInstance()->replaceScene(transition);
                }
                });

            // Añadir el botón Game Over a la escena
            this->addChild(gameOverButton);
        }
        break;
    case DIR_DEF::RIGHT:
        head->col++;
        head->nodeSprite->setRotation(-90);
        head->nodeSprite->setAnchorPoint(Point::ANCHOR_TOP_LEFT);
        // Si sale por la derecha, que aparezca por la izquierda
        if (head->col >= 16) {
            head->nodeSprite->setVisible(false);
            for (auto sn : body) {
                sn->nodeSprite->setVisible(false);
            }
            food->nodeSprite->setVisible(false);
            Label* scoreLabel = Label::createWithTTF("Puntaje: " + std::to_string(body.size()), "fonts/Marker Felt.ttf", 24);
            scoreLabel->setPosition(Vec2(100, 100));
            this->addChild(scoreLabel);
            // Crear el botón Game Over
            Size visibleSize = Director::getInstance()->getVisibleSize();
            auto gameOverButton = ui::Button::create("background.png");
            gameOverButton->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));

            // Configurar la acción del botón para regresar al menú principal (MainMenu)
            gameOverButton->addTouchEventListener([=](Ref* pSender, ui::Widget::TouchEventType type) {
                if (type == ui::Widget::TouchEventType::ENDED) {
                    // Crear una transición hacia la escena HelloWorld (MainMenu)
                    auto scene = HelloWorld::createScene();
                    auto transition = TransitionFade::create(1.0f, scene);
                    Director::getInstance()->replaceScene(transition);
                }
                });

            // Añadir el botón Game Over a la escena
            this->addChild(gameOverButton);
        }
        break;
    }



    // Actualizar la posición de la cabeza
    head->nodeSprite->setPosition(Point(head->col * 32, head->row * 32));

    // Verificar si la cabeza come la comida
    if (head->col == food->col && head->row == food->row) {
        food->col = rand() % 10 + 2;
        food->row = rand() % 10 + 2;
        food->nodeSprite->setPosition(Point(food->col * 32, food->row * 32));

        auto sn = new SnakeNode();
        if (body.size() > 0) {
            sn->Prenode = body.back();
        }
        else {
            sn->Prenode = head;
        }

        switch (sn->Prenode->dir) {
        case UP:
            sn->row = sn->Prenode->row - 1;
            sn->col = sn->Prenode->col;
            break;
        case DOWN:
            sn->row = sn->Prenode->row + 1;
            sn->col = sn->Prenode->col;
            break;
        case LEFT:
            sn->row = sn->Prenode->row;
            sn->col = sn->Prenode->col + 1;
            break;
        case RIGHT:
            sn->row = sn->Prenode->row;
            sn->col = sn->Prenode->col - 1;
            break;
        }
        sn->nodeSprite = Sprite::create("body.png");
        sn->nodeSprite->setAnchorPoint(Point::ANCHOR_BOTTOM_LEFT);
        sn->nodeSprite->setPosition(Point(sn->col * 32, sn->row * 32));
        this->addChild(sn->nodeSprite);

        body.pushBack(sn);
        float newSpeed = 0.2f - (body.size() * 0.01f); // Aumenta la velocidad al comer
        newSpeed = std::max(newSpeed, 0.1f); // Limitar la velocidad mínima
        this->unschedule("update_key");
        this->schedule([this](float delta) {
            this->update(delta);
            }, newSpeed, "update_key");

    }

    // Verificar si la cabeza choca con su cuerpo
    for (auto i = body.size() - 1; i >= 0; i--) {
        auto sn = body.at(i);
        if (head->col == sn->col && head->row == sn->row) {
            // Pausar el juego
            head->nodeSprite->setVisible(false);
            for (auto sn : body) {
                sn->nodeSprite->setVisible(false);
            }
            food->nodeSprite->setVisible(false);
            Label* scoreLabel = Label::createWithTTF("Puntaje: " + std::to_string(body.size()), "fonts/Marker Felt.ttf", 24);
            scoreLabel->setPosition(Vec2(100, 100));
            this->addChild(scoreLabel);
            // Crear el botón Game Over
            Size visibleSize = Director::getInstance()->getVisibleSize();
            auto gameOverButton = ui::Button::create("background.png");
            gameOverButton->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));

            // Configurar la acción del botón para regresar al menú principal (MainMenu)
            gameOverButton->addTouchEventListener([=](Ref* pSender, ui::Widget::TouchEventType type) {
                if (type == ui::Widget::TouchEventType::ENDED) {
                    // Crear una transición hacia la escena HelloWorld (MainMenu)
                    auto scene = HelloWorld::createScene();
                    auto transition = TransitionFade::create(1.0f, scene);
                    Director::getInstance()->replaceScene(transition);
                }
                });

            // Añadir el botón Game Over a la escena
            this->addChild(gameOverButton);

            break;  // Termina el bucle ya que el juego terminó
        }
    }





}
