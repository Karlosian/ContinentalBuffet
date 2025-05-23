#ifndef RECIPE_H_
#define RECIPE_H_

#include <string>
#include <vector>

#include "Ingredient.h"
#include "CookingProcess.h"

class Recipe {
public:
    std::string name;
    std::string description;

    std::vector<Ingredient> recipeIngredients;
    std::vector<CookingProcess> steps;

    static std::vector<Recipe> recipes;
public:
    // Constructors
    Recipe();
    Recipe(std::string n, std::string d, std::vector<Ingredient> i, std::vector<CookingProcess> s);

    // Getters
    std::vector<Ingredient> getIngredients();
    std::vector<Ingredient> getSteps();

    static void getRecipeList();
};

#endif  // RECIPE_H_
