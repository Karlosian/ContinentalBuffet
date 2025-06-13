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

class Shop : public ax::Scene {
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
    GameState _gameState                            = GameState::init;
    ax::EventListenerTouchAllAtOnce* _touchListener = nullptr;
    ax::EventListenerKeyboard* _keyboardListener    = nullptr;
    ax::EventListenerMouse* _mouseListener          = nullptr;
    int _sceneID                                    = 0;

    fairygui::GRoot* root;
    fairygui::GList* ingredientsList;
    fairygui::GTextField* costLabel;
    fairygui::GTextField* balanceLabel;

    std::vector<Ingredient> cart;

    double totalCost = 0;

public:
    // Initialization Methods
    void loadStartScreen();
    bool init() override;
    void update(float delta) override;
    void menuCloseCallback(ax::Object* sender);

    // Render Methods
    void renderListItems(int index, fairygui::GObject* obj);

    // Dynamic GUI Methods
    void addIngredientToCart(Ingredient ingredient);
    void sortIngredientInCart();

    // Debugging Methods
    void print();

    // Overload toString method
    std::string toString(double c) const;

    Shop();
    ~Shop() override;
};

#endif  // !SHOP_H_
