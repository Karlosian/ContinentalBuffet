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
#include "../Classes/Player.h"

#include <iostream>
#include <iomanip>
#include <sstream>
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
    if (index >= 0 && index < cart.size()) 
    {
        fairygui::GComponent* itemComponent = obj->as<fairygui::GComponent>();
        fairygui::GComponent* itemCost = obj->as<fairygui::GComponent>();
        if (itemComponent) 
        {
            fairygui::GTextField* label = itemComponent->getChild("n0")->as<fairygui::GTextField>();
            if (label)
                label->setText(cart[index].getName() + " x " + std::to_string(cart[index].getQuantity()));
            
            fairygui::GTextField* itemCostLabel = itemCost->getChild("n1")->as<fairygui::GTextField>();
            if (itemCostLabel)
                itemCostLabel->setText(toString(cart[index].getCost() * cart[index].getQuantity()));
        }
    }
}

void Shop::addIngredientToCart(Ingredient ingredient)
{
    for (auto& item : cart) {
        if (item.getName() == ingredient.getName())
        { 
            item.changeQuantityBy(1);
            std::cout << item.getQuantity();
            totalCost += ingredient.getCost();
            costLabel->setText(toString(totalCost) + "$");
            return;
        }
    }
    ingredient.setQuantity(1);
    cart.push_back(ingredient);  // Add the ingredient to the cart vector
    //sortIngredientInCart();

    std::cout << "Added " << ingredient.getName() << " to cart. Cart size: " << cart.size() << "cost: " << ingredient.getCost() << std::endl;
    totalCost += ingredient.getCost();

    std::cout << "Cost: " << totalCost << std::endl;
    costLabel->setText(toString(totalCost) + "$");
}

// Overload toString method to ensure two decimal places
std::string Shop::toString(double c) const {
    std::stringstream ss;
    ss << std::fixed << std::setprecision(2) << c;
    return ss.str();
}

// Using bubble sort to sort the ingredients in the cart
void Shop::sortIngredientInCart() {
    for (int j = 0; j < cart.size() - 1; j++) {
        if (cart[j].getName() > cart[j + 1].getName()) {
            auto temp = cart[j];
            cart[j] = cart[j + 1];
            cart[j + 1] = temp;
        }
    }
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
    ingredientsList->setNumItems(cart.size());

    fairygui::GObject* costLabelComponent = shopComponent->getChild("n15");
    if (costLabelComponent == nullptr || costLabelComponent->as<fairygui::GTextField>() == nullptr) {
        std::cout << "Cost GLabel broke \n";
        return;
    }
    costLabel = costLabelComponent->as<fairygui::GTextField>();

    for (int i = 1; i <= 53; i++)
    {
        std::string temp = "b" + std::to_string(i);
        fairygui::GObject* ingredientsButton1 = ingredientsComponent->getChild(temp);
        if (ingredientsButton1 != nullptr && ingredientsButton1->as<fairygui::GButton>() != nullptr) {
            fairygui::GButton* rightArrow = ingredientsButton1->as<fairygui::GButton>();
            rightArrow->addClickListener([this, i](fairygui::EventContext* context) {
                std::vector<Ingredient> allIngredient = Ingredient::getIngredients();
                this->addIngredientToCart(allIngredient[i - 1]);
                ingredientsList->setNumItems(cart.size());
            });
        }
    }
    fairygui::GObject* clearButton = shopComponent->getChild("n18");
    if (clearButton != nullptr && clearButton->as<fairygui::GButton>() != nullptr) {
        fairygui::GButton* clear = clearButton->as<fairygui::GButton>();
        clear->addClickListener([this](fairygui::EventContext* context) {
            totalCost = 0;
            costLabel->setText(toString(totalCost) + "$");
            while (!cart.empty()) {
                cart.pop_back();
            }
            ingredientsList->setNumItems(0);
        });
    }

    fairygui::GObject* buyButton = shopComponent->getChild("n17");
    if (buyButton != nullptr && buyButton->as<fairygui::GButton>() != nullptr)
    {
        fairygui::GButton* buy = buyButton->as<fairygui::GButton>();
        buy->addClickListener([this](fairygui::EventContext* context) {
            Player::setMoney(totalCost);
            totalCost = 0;
            costLabel->setText(toString(totalCost) + "$");
            for (auto& item : cart) Player::addIngredient(item);
            while (!cart.empty()) cart.pop_back();
            ingredientsList->setNumItems(0);
        });
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
