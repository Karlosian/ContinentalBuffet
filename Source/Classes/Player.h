#ifndef PLAYER_H_
#define PLAYER_H_

#include <vector>
#include "Ingredient.h"

class Player {
private:
    static Player *instance;                    // Singleton instance
    static std::vector<Ingredient> inventory;   // Player's inventory
    static std::vector<bool> ingredientsChosen; 
    static double money;                               // Player's money
public:
    static Player* getInstance();

    // Getters
    static std::vector<Ingredient> getInventory();
    static std::vector<bool> getIngredientsChosen();
    static double getMoney();

    // Setters
    static void setIngredientsChosen(int index, bool isChosen);
    static void setMoney(double i);

    // Methods
    static void addIngredient(Ingredient ingredient);
    static void sortInventory();
    static void sortInventory(int low, int high);

    // Test
    static void initalizeTestInventory();
};

#endif
