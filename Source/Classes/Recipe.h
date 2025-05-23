#ifndef RECIPE_H_
#define RECIPE_H_

#include <string>
#include <vector>

#include "Ingredient.h"
//#include "Axmol.h"

class Recipe {
public:
    std::string name;
    std::string description;
    std::vector<Ingredient> ingredients;
    std::vector<std::string> steps;
public:
    // Getters
    std::vector<Ingredient> getIngredients();
    std::vector<Ingredient> getSteps();
};

#endif  // RECIPE_H_
