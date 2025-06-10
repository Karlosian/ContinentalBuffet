#include "Homepage.h"
#include "CookingScene.h"
#include "Shop.h"

#include "../Classes/Ingredient.h"
#include "../Classes/Player.h"
#include "../AppDelegate.h"


#include "GRoot.h"
#include "UIPackage.h"
#include "GComponent.h"
#include "GButton.h"
#include "BubbleGame.h"

#include <iostream>

using namespace ax;

static int s_sceneID = 2000;

void Homepage::loadStartScreen() {
    fairygui::UIPackage::addPackage("UI/Package1");
    fairygui::GComponent* homepageComponent = fairygui::UIPackage::createObject("Package1", "Component1")->as<fairygui::GComponent>();
    homepageComponent->setPosition(0, 0);
    root->addChild(homepageComponent);

    fairygui::GObject* startButtonObject = homepageComponent->getChild("n4");

    Ingredient::getIngredientList();
    Recipe::getRecipeList();
    Player::initalizeTestInventory();

    if (startButtonObject != nullptr && startButtonObject->as<fairygui::GButton>() != nullptr) {
        fairygui::GButton* startButton = startButtonObject->as<fairygui::GButton>();
        startButton->addClickListener([](fairygui::EventContext* context) {
            Director::getInstance()->replaceScene(utils::createInstance<BubbleGame>());
        });
    }
}

bool Homepage::init() {
    if (!Scene::init()) return false;

    auto visibleSize = _director->getVisibleSize();
    auto origin      = _director->getVisibleOrigin();
    auto safeArea    = _director->getSafeAreaRect();
    auto safeOrigin  = safeArea.origin;

    root = fairygui::GRoot::create(this);
    root->retain();
    loadStartScreen();

    scheduleUpdate();
    return true;
}

void Homepage::update(float delta) {
    
}

void Homepage::menuCloseCallback(ax::Object* sender) {
    _director->end();
}

Homepage::Homepage() {
    _sceneID = s_sceneID++;
}

Homepage::~Homepage() {
    _sceneID = -1;
}
