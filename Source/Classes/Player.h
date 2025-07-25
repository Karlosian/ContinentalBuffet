#ifndef PLAYER_H_
#define PLAYER_H_

#include <vector>
#include "Ingredient.h"
#include "Recipe.h"

class Player {
private:
    static Player *instance;                    // Singleton instance
    static std::vector<Ingredient> inventory;   // Player's inventory
    static std::vector<bool> ingredientsChosen; // Bool vector for which ingredients are chosen during the cooking scene
    static std::vector<Recipe> playerRecipes;   // Player's recipes
    static double money;                        // Player's money
public:
    static Player* getInstance();

    // Getters
    static std::vector<Ingredient> getInventory();
    static std::vector<bool> getIngredientsChosen();
    static double getMoney();
    static std::vector<Recipe> getPlayerRecipes();

    // Setters
    static void setIngredientsChosen(int index, bool isChosen);
    static bool setMoney(double i);
    static void addMoney(double i);
    static void decreaseIngredientQuantity(int index, int amount);
    static void addRecipeToInventory(Recipe recipe);

    // Methods
    static void addIngredient(Ingredient ingredient);
    static void sortInventory();
    static void sortInventory(int low, int high);

    // Test
    static void initalizeTestInventory();
};

#endif
