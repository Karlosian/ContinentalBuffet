#ifndef RECIPE_H_
#define RECIPE_H_

#include <string>
#include <vector>

#include "Ingredient.h"
#include "CookingProcess.h"

// This class represents the recipe of a dish, including its name, description, ingredients, and cooking steps.
class Recipe {
public:
    // Name of the recipe
    std::string name;

    // Short description of the recipe (for hover on the recipe book)
    std::string description;

    // List of ingredients needed for the recipe
    std::vector<Ingredient> recipeIngredients;

    // List of cooking steps for the recipe (e.g., "Chop the onions", "Boil the water", etc.)
    std::vector<CookingProcess> steps;

    // Static vector to hold all recipes in the game
    static std::vector<Recipe> recipes;
public:
    // Default Constructors
    Recipe();

    // Parameterized Constructor
    Recipe(std::string n, std::string d, std::vector<Ingredient> i, std::vector<CookingProcess> s);

    // Getters
    std::string getRecipeName();
    std::vector<Ingredient> getRecipeIngredients();
    std::vector<CookingProcess> getRecipeSteps();

    // Function to extract all the recipes from the JSON file and store them in the static vector
    static void getRecipeList();
};

#endif  // RECIPE_H_
