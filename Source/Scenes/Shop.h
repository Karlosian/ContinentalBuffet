#ifndef SHOP_H_
#define SHOP_H_

#include "axmol.h"
#include "vector"

#include "GRoot.h"
#include "UIPackage.h"
#include "GComponent.h"
#include "GList.h"
#include "GButton.h"
#include "GTextField.h"

#include "../Classes/Ingredient.h"

class Shop : public ax::Scene
{
public:
    enum class GameState
    {
        init = 0,
        update,
        pause,
        end,
        menu1,
        menu2,
    };

private:
    GameState _gameState = GameState::init;

    // Event listeners for user inputs
    ax::EventListenerTouchAllAtOnce* _touchListener = nullptr;
    ax::EventListenerKeyboard* _keyboardListener    = nullptr;
    ax::EventListenerMouse* _mouseListener          = nullptr;

    int _sceneID = 0;

    // FairyGUI UI components
    fairygui::GRoot* root;
    fairygui::GList* ingredientsList;
    fairygui::GTextField* costLabel;
    fairygui::GTextField* balanceLabel;

    std::vector<Ingredient> cart;  // Shopping cart for ingredients
    double totalCost = 0;

public:
    // Scene lifecycle methods
    void loadStartScreen();
    bool init() override;
    void update(float delta) override;
    void menuCloseCallback(ax::Object* sender);

    // Rendering and UI logic
    void renderListItems(int index, fairygui::GObject* obj);
    void addIngredientToCart(Ingredient ingredient);
    void sortIngredientInCart();

    // Utility
    void print();                          // Debugging print function
    std::string toString(double c) const;  // Converts double to string

    Shop();
    ~Shop() override;
};

#endif  // !SHOP_H_
