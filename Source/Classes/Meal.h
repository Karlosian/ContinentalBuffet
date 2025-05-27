#ifndef MEAL_H_
#define MEAL_H_

#include <vector>

#include "Recipe.h"
#include "Ingredient.h"
#include "CookingProcess.h"

class Meal : public Recipe {
private:
    double recipeAccuracy;

public:
    Meal();
    Meal(std::vector<Ingredient> i, std::vector<CookingProcess> cp);
    
    Recipe findMatchingRecipe();
};

#endif  // !MEAL_H_
