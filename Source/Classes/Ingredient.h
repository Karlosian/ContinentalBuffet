#ifndef INGREDIENT_H_
#define INGREDIENT_H_

#include <string>
#include <vector>

class Ingredient {
private:
    std::string name, unit;
    int amount, quantity, type;
    double cost;
    static std::vector<Ingredient> ingredients;

public:

    // Constructors
    Ingredient();
    Ingredient(std::string n, int a, std::string u);
    Ingredient(std::string n, std::string u, double c, int a, int t);

    // Getters
    std::string getName();
    double getCost();
    int getAmount();
    int getQuantity();

    static std::vector<Ingredient> getIngredients();
    static void getIngredientList(); // Extracting the ingredients from ingredients.json

    // Setters
    void setQuantity(int q);
    void changeQuantityBy(int q);

    // Find
    std::string* findRecipes();

};

#endif
