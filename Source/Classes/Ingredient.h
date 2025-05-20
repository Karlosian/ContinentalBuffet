#ifndef INGREDIENT_H_

#include <string>

class Ingredient {
private:
    std::string name;
    int cost, quantity, type;
    static std::string *typeOfIngredientName;
public:
    // Getters
    int getCost();
    int getQuantity();

    // Setters
    void setQuantity(int q);
    void changeQuantityBy(int q);

    // Find
    std::string* findRecipes();
};

#endif  // !_INGREDIENT_H_
