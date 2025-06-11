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

void Shop::renderListItems(int index, fairygui::GObject* obj)
{
    if (index >= 0 && index < labels.size()) 
    {
        fairygui::GComponent* itemComponent = obj->as<fairygui::GComponent>();
        if (itemComponent) 
        {
            fairygui::GTextField* label = itemComponent->getChild("n0")->as<fairygui::GTextField>();
            if (label) label->setText(labels[index]);
        }
    }
}

void Shop::addIngredientToCart(const Ingredient& ingredient)
{
    cart.push_back(ingredient);  // Add the ingredient to the cart vector
    std::cout << "Added " << ingredient.getName() << " to cart. Cart size: " << cart.size() << std::endl;
}

void Shop::loadStartScreen() {
    fairygui::UIPackage::addPackage("UI/Shop");
    fairygui::GComponent* shopComponent = fairygui::UIPackage::createObject("Package2", "Component1")->as<fairygui::GComponent>();
    shopComponent->setPosition(0, 0);
    root->addChild(shopComponent);
    fairygui::GObject* ingredientsComponentObject = shopComponent->getChild("n11");

    fairygui::GComponent* ingredientsComponent = ingredientsComponentObject->as<fairygui::GComponent>();

    ingredientsList = shopComponent->getChild("n12")->as<fairygui::GList>();
    ingredientsList->itemRenderer = CC_CALLBACK_2(Shop::renderListItems, this);
    ingredientsList->setNumItems(labels.size());

    for (int i = 1; i <= 53; i++)
    {
        std::string temp = "b" + std::to_string(i);
        fairygui::GObject* ingredientsButton1 = ingredientsComponent->getChild(temp);
        if (ingredientsButton1 != nullptr && ingredientsButton1->as<fairygui::GButton>() != nullptr) {
            fairygui::GButton* rightArrow = ingredientsButton1->as<fairygui::GButton>();
            rightArrow->addClickListener([this, i](fairygui::EventContext* context) {
                std::vector<Ingredient> allIngredient = Ingredient::getIngredients();
                labels.push_back(allIngredient[i - 1].getName());
                ingredientsList->setNumItems(labels.size());
                this->addIngredientToCart(allIngredient[i - 1]);
            });
        }
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
