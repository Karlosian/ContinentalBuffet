#ifndef INGREDIENT_H_
#define INGREDIENT_H_

#include <string>
#include <vector>

class Ingredient {
private:
    std::string name, unit;
    int amount, type;   // Amount refers to how many of the ingredient is included for every 1 purchase of that ingredient
    int quantity;       // Quantity refers to how many of these bundles of "Amount" were purchased (so in total, there are Amount x Quantity of that ingredient)
    double cost;
    int nameIndex;  // Index of the ingredient in the ingredients.json file

    // Vector to hold all ingredients from ingredients.json
    static std::vector<Ingredient> ingredients;

public:
    // Constructors
    Ingredient();
    Ingredient(std::string n, int a, std::string u);
    Ingredient(std::string n, std::string u, double c, int a, int t, int i);
    Ingredient(Ingredient& i, int q);  // Copy constructor

    // Getters
    std::string getName();
    double getCost();
    int getAmount();
    int getQuantity();
    int getNameIndex();
    static std::vector<Ingredient> getIngredients();

    // Extracting the ingredients from ingredients.json
    static void getIngredientList();

    // Setters
    void setQuantity(int q);
    void changeQuantityBy(int q);
    void setNameIndex(int i);

    // Overload assignment operator
    void operator=(const Ingredient& i);
};

#endif
