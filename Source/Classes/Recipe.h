#ifndef RECIPE_H_
#define RECIPE_H_

#include <string>
#include <vector>

#include "Ingredient.h"
#include "CookingProcess.h"
#include "axmol.h"
// This class represents the recipe of a dish, including its name, description, ingredients, and cooking steps.
class Recipe {
protected:
    // Name of the recipe
    std::string name;

    // Short description of the recipe (for hover on the recipe book)
    std::string description;

    // Cost of the meal made from that recipe
    double cost;

    // List of ingredients needed for the recipe
    std::vector<Ingredient> recipeIngredients;

    // List of cooking steps for the recipe (e.g., "Chop the onions", "Boil the water", etc.)
    std::vector<CookingProcess> steps;

private:
    // Static vector to hold all recipes in the game
    static std::vector<Recipe> recipes;
    
public:
    // Default Constructors
    Recipe();

    // Parameterized Constructor
    Recipe(std::string n, std::string d, double c, std::vector<Ingredient> i, std::vector<CookingProcess> s);

    // Getters
    std::string getRecipeName();
    std::vector<Ingredient> getRecipeIngredients();
    std::vector<CookingProcess> getRecipeSteps();
    std::string getDescription();
    static std::vector<Recipe> getAllRecipes();
    double getCost();

    // Function to extract all the recipes from the JSON file and store them in the static vector
    static void getRecipeList();

    // Overload equal operator
    Recipe* operator=(const Recipe& other);
};

#endif  // RECIPE_H_
