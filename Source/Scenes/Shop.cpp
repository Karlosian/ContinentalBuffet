#include "Shop.h"
#include "CookingScene.h"

#include "GRoot.h"
#include "UIPackage.h"
#include "GComponent.h"
#include "GButton.h"
#include "GList.h"
#include "GLabel.h"
#include "GTextField.h"
#include "../Classes/Ingredient.h"

#include <iostream>
#include <string>

using namespace ax;

static int s_sceneID = 2000;
static int itemCounter = 0;

void Shop::print()
{

    std::cout << "RAN";
}

void Shop::loadStartScreen() {
    fairygui::UIPackage::addPackage("UI/Shop");
    fairygui::GComponent* shopComponent = fairygui::UIPackage::createObject("Package2", "Component1")->as<fairygui::GComponent>();
    shopComponent->setPosition(0, 0);
    root->addChild(shopComponent);
    fairygui::GObject* ingredientsComponentObject = shopComponent->getChild("n11");

    fairygui::GComponent* ingredientsComponent = ingredientsComponentObject->as<fairygui::GComponent>();

    fairygui::GObject* ingredientsButton1 = ingredientsComponent->getChild("b1");

    ingredientsList = shopComponent->getChild("n12")->as<fairygui::GList>();

    // Initialize list settings
    if (ingredientsList)
    {
        ingredientsList->removeChildrenToPool();  // Clear existing items

        // Set default item URL (create this in FairyGUI editor first!)
        ingredientsList->setDefaultItem("ui://Package2/ListItem");
    }

    if (ingredientsButton1 != nullptr && ingredientsButton1->as<fairygui::GButton>() != nullptr)
    {
        fairygui::GButton* rightArrow = ingredientsButton1->as<fairygui::GButton>();
        rightArrow->addClickListener(
            [this](fairygui::EventContext* context) { this->print(); });
    }

}

bool Shop::init() {
    if (!Scene::init())
        return false;

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

void Shop::update(float delta) {}

void Shop::menuCloseCallback(ax::Object* sender)
{
    _director->end();
}

Shop::Shop()
{
    _sceneID = s_sceneID++;
}

Shop::~Shop()
{
    _sceneID = -1;
}
