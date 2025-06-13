#include "Shop.h"
#include "CookingScene.h"

// FairyGUI UI components
#include "GRoot.h"
#include "UIPackage.h"
#include "GComponent.h"
#include "GButton.h"
#include "GList.h"
#include "GLabel.h"
#include "GTextField.h"

#include "../Classes/Ingredient.h"  // Ingredient class
#include "../Classes/Player.h"      // Player class

#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>

using namespace ax;

static int s_sceneID = 2000;

void Shop::print()
{
    std::cout << "RAN";  // Debug print
}

// Renders individual items in the cart display list.
void Shop::renderListItems(int index, fairygui::GObject* obj)
{
    if (index >= 0 && index < cart.size())
    {
        fairygui::GComponent* itemComponent = obj->as<fairygui::GComponent>();
        if (itemComponent)
        {
            // Set ingredient name/quantity and total cost for the item.
            itemComponent->getChild("n0")->as<fairygui::GTextField>()->setText(
                cart[index].getName() + " x " + std::to_string(cart[index].getQuantity()));
            itemComponent->getChild("n1")->as<fairygui::GTextField>()->setText(
                toString(cart[index].getCost() * cart[index].getQuantity()));
        }
    }
}

// Adds an ingredient to the shopping cart, updating quantities if it already exists.
void Shop::addIngredientToCart(Ingredient ingredient)
{
    for (auto& item : cart)
    {
        if (item.getName() == ingredient.getName())
        {
            item.changeQuantityBy(1);
            sortIngredientInCart();
            totalCost += ingredient.getCost();
            costLabel->setText(toString(totalCost) + "$");
            return;
        }
    }
    ingredient.setQuantity(1);
    cart.push_back(ingredient);
    sortIngredientInCart();

    totalCost += ingredient.getCost();
    costLabel->setText(toString(totalCost) + "$");
}

// Converts a double to a string with two decimal places.
std::string Shop::toString(double c) const
{
    std::stringstream ss;
    ss << std::fixed << std::setprecision(2) << c;
    return ss.str();
}

// Sorts ingredients in the cart alphabetically by name using bubble sort.
void Shop::sortIngredientInCart()
{
    for (size_t i = 0; i < cart.size() - 1; i++)
    {
        for (size_t j = 0; j < cart.size() - 1 - i; j++)
        {
            if (cart[j].getName() > cart[j + 1].getName())
            {
                std::swap(cart[j], cart[j + 1]);
            }
        }
    }
}

// Loads and configures the shop's user interface.
void Shop::loadStartScreen()
{
    fairygui::UIPackage::addPackage("UI/Shop");
    fairygui::GComponent* shopComponent =
        fairygui::UIPackage::createObject("Package2", "Component1")->as<fairygui::GComponent>();
    root->addChild(shopComponent);

    // Get and configure the ingredients list.
    ingredientsList = shopComponent->getChild("n12")->as<fairygui::GList>();
    ingredientsList->itemRenderer = CC_CALLBACK_2(Shop::renderListItems, this);
    ingredientsList->setNumItems(cart.size());

    // Get and assign cost and balance labels.
    costLabel = shopComponent->getChild("n15")->as<fairygui::GTextField>();
    balanceLabel = shopComponent->getChild("n16")->as<fairygui::GTextField>();
    if (balanceLabel)
        balanceLabel->setText(toString(Player::getMoney()) + "$");

    // Attach click listeners to ingredient buy buttons.
    fairygui::GComponent* ingredientsComponent = shopComponent->getChild("n11")->as<fairygui::GComponent>();
    if (ingredientsComponent)
    {
        for (int i = 1; i <= 53; i++)
        {
            fairygui::GButton* buyButton =
                ingredientsComponent->getChild("b" + std::to_string(i))->as<fairygui::GButton>();
            if (buyButton)
            {
                buyButton->addClickListener([this, i](fairygui::EventContext* context) {
                    this->addIngredientToCart(Ingredient::getIngredients()[i - 1]);
                    ingredientsList->setNumItems(cart.size());
                });
            }
        }
    }

    // "Clear Cart" button listener.
    fairygui::GButton* clearButton = shopComponent->getChild("n18")->as<fairygui::GButton>();
    if (clearButton)
    {
        clearButton->addClickListener([this](fairygui::EventContext* context) {
            totalCost = 0;
            costLabel->setText(toString(totalCost) + "$");
            cart.clear();
            ingredientsList->setNumItems(0);
        });
    }

    // "Buy" button listener.
    fairygui::GButton* buyButton = shopComponent->getChild("n17")->as<fairygui::GButton>();
    if (buyButton)
    {
        buyButton->addClickListener([this](fairygui::EventContext* context) {
            Player::setMoney(totalCost);
            balanceLabel->setText(toString(Player::getMoney()) + "$");
            totalCost = 0;
            costLabel->setText(toString(totalCost) + "$");
            for (auto& item : cart)
                Player::addIngredient(item);
            cart.clear();
            ingredientsList->setNumItems(0);
        });
    }

    // "Back" button listener.
    fairygui::GButton* backButton = shopComponent->getChild("n19")->as<fairygui::GButton>();
    if (backButton)
    {
        backButton->addClickListener([this](fairygui::EventContext* context) {
            Director::getInstance()->replaceScene(utils::createInstance<CookingScene>());
        });
    }
}

// Scene initialization.
bool Shop::init()
{
    if (!Scene::init())
        return false;
    root = fairygui::GRoot::create(this);
    root->retain();
    loadStartScreen();
    scheduleUpdate();
    return true;
}

void Shop::update(float delta) {}  // Per-frame update (currently no logic).

// Closes the application.
void Shop::menuCloseCallback(ax::Object* sender)
{
    _director->end();
}

// Constructor: Assigns unique scene ID.
Shop::Shop()
{
    _sceneID = s_sceneID++;
}

// Destructor.
Shop::~Shop()
{
    _sceneID = -1;
}
