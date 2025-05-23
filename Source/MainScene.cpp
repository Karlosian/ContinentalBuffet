/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.
 Copyright (c) 2019-present Axmol Engine contributors (see AUTHORS.md).

 https://axmol.dev/

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "MainScene.h"
#include "GRoot.h"
#include "UIPackage.h"
#include "GComponent.h"

#include "Classes/Ingredient.h"
#include "Classes/Recipe.h"

#include <iostream>
#include <string>
using namespace ax;

static int s_sceneID = 1000;

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf(
        "Depending on how you compiled you might have to add 'Content/' in front of filenames in "
        "MainScene.cpp\n");
}

// on "init" you need to initialize your instance
bool MainScene::init()
{
    //////////////////////////////
    // 1. super init first
    if (!Scene::init()) return false;

    auto visibleSize = _director->getVisibleSize();
    auto origin      = _director->getVisibleOrigin();
    auto safeArea    = _director->getSafeAreaRect();
    auto safeOrigin  = safeArea.origin;

    // Load JSON file
    Ingredient::getIngredientList();
    Recipe::getRecipeList();

    // Test
    auto background = Sprite::create("Wardle.png");
    background->setPosition(origin.x + visibleSize.x / 2, origin.y + visibleSize.y / 2);
    this->addChild(background);

    // Test
	fairygui::GRoot* root = fairygui::GRoot::create(this);
    root->retain();

    fairygui::UIPackage::addPackage("UI/Package1");
    fairygui::GComponent* myComponent = fairygui::UIPackage::createObject("Package1", "Component1")->as<fairygui::GComponent>();
    myComponent->setPosition(0, 0);
    root->addChild(myComponent);

    fairygui::UIPackage::addPackage("UI/Package2");
    fairygui::GComponent* myComponent2 = fairygui::UIPackage::createObject("Package2", "Component1")->as<fairygui::GComponent>();
    myComponent2->setPosition(0, 0);
    //root->addChild(myComponent2);

    /////////////////////////////
    // 3. add your codes below...
    _keyboardListener                = EventListenerKeyboard::create();
    _keyboardListener->onKeyPressed  = AX_CALLBACK_2(MainScene::onKeyPressed, this);
    _keyboardListener->onKeyReleased = AX_CALLBACK_2(MainScene::onKeyReleased, this);
    _eventDispatcher->addEventListenerWithFixedPriority(_keyboardListener, 11);

    // scheduleUpdate() is required to ensure update(float) is called on every loop
    scheduleUpdate();

    return true;
}

void MainScene::onMouseDown(Event* event)
{
    EventMouse* e = static_cast<EventMouse*>(event);
    // AXLOGD("onMouseDown detected, Key: %d", static_cast<int>(e->getMouseButton()));
}

void MainScene::onMouseUp(Event* event)
{
    EventMouse* e = static_cast<EventMouse*>(event);
    AXLOGD("onMouseUp detected, Key: %d", static_cast<int>(e->getMouseButton()));
}

void MainScene::onMouseMove(Event* event)
{
    EventMouse* e = static_cast<EventMouse*>(event);
    // AXLOGD("onMouseMove detected, X:{}  Y:{}", e->getCursorX(), e->getCursorY());
}

void MainScene::onMouseScroll(Event* event)
{
    EventMouse* e = static_cast<EventMouse*>(event);
    // AXLOGD("onMouseScroll detected, X:{}  Y:{}", e->getScrollX(), e->getScrollY());
}

void MainScene::onKeyPressed(EventKeyboard::KeyCode code, Event* event)
{
    AXLOGD("Scene: #{} onKeyPressed, keycode: {}", _sceneID, static_cast<int>(code));
}

void MainScene::onKeyReleased(EventKeyboard::KeyCode code, Event* event)
{
    AXLOGD("onKeyReleased, keycode: %d", static_cast<int>(code));
}

void MainScene::update(float delta)
{
    switch (_gameState)
    {
    case GameState::init:
    {
        _gameState = GameState::update;
        break;
    }

    case GameState::update:
    {
        /////////////////////////////
        // Add your codes below...like....
        //
        // UpdateJoyStick();
        // UpdatePlayer();
        // UpdatePhysics();
        // ...
        break;
    }

    case GameState::pause:
    {
        /////////////////////////////
        // Add your codes below...like....
        //
        // anyPauseStuff()

        break;
    }

    case GameState::menu1:
    {  /////////////////////////////
        // Add your codes below...like....
        //
        // UpdateMenu1();
        break;
    }

    case GameState::menu2:
    {  /////////////////////////////
        // Add your codes below...like....
        //
        // UpdateMenu2();
        break;
    }

    case GameState::end:
    {  /////////////////////////////
        // Add your codes below...like....
        //
        // CleanUpMyCrap();
        menuCloseCallback(this);
        break;
    }

    }  // switch
}

void MainScene::menuCloseCallback(ax::Object* sender)
{
    // Close the axmol game scene and quit the application
    _director->end();

    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use
     * _director->end() as given above,instead trigger a custom event created in RootViewController.mm
     * as below*/

    // EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);
}

MainScene::MainScene()
{
    _sceneID = ++s_sceneID;
    AXLOGD("Scene: ctor: #{}", _sceneID);
}

MainScene::~MainScene()
{
    AXLOGD("~Scene: dtor: #{}", _sceneID);

    if (_touchListener)
        _eventDispatcher->removeEventListener(_touchListener);
    if (_keyboardListener)
        _eventDispatcher->removeEventListener(_keyboardListener);
    if (_mouseListener)
        _eventDispatcher->removeEventListener(_mouseListener);
    _sceneID = -1;
}
