#include <iostream>
#include <fstream>
#include <filesystem>
#include <vector>

#include "Recipe.h"
#include "Ingredient.h"
#include "CookingProcess.h"  // Renamed from Action.h

#define JSON_HAS_CPP_20 0
#include "json.hpp"

std::vector<Recipe> Recipe::recipes;  // Definition for static member

Recipe::Recipe() {}

Recipe::Recipe(std::string n, std::string d, std::vector<Ingredient> i, std::vector<CookingProcess> s)
{
    name              = n;
    description       = d;
    recipeIngredients = i;
    steps             = s;
}

void Recipe::getRecipeList()
{
    std::ifstream file("recipes.json");
    std::cout << "Current working directory: " << std::filesystem::current_path() << std::endl;

    if (!file.is_open())
    {
        std::cerr << "Error opening recipe file" << std::endl;
        return;
    }

    std::cout << "Reading recipes from JSON file..." << std::endl;

    try
    {
        nlohmann::json json_data;
        file >> json_data;

        for (const auto& recipe : json_data)
        {
            std::vector<Ingredient> recipeIngredients;
            // Populating recipeIngredients from the top-level "ingredients" array
            for (const auto& ingredient_item : recipe["ingredients"])
            {
                recipeIngredients.push_back(
                    Ingredient(ingredient_item["name"], ingredient_item["amount"], ingredient_item["unit"]));
            }

            std::vector<CookingProcess> steps;
            for (const auto& step : recipe["steps"])
            {
                std::vector<Ingredient> ingredients_for_step;  // Renamed for clarity

                // FIX: Remove incorrect line and explicitly get string value for comparison
                // std::string name = step["name"]; // This line was incorrect and removed.

                // FIX: Explicitly convert the JSON value to a std::string for comparison
                for (const auto& ingredient_json_value : step["ingredients"])
                {
                    std::string ingredient_name_from_step =
                        ingredient_json_value.get<std::string>();  // Get the string value

                    for (Ingredient& foundIngredient : recipeIngredients)
                    {
                        // FIX: Compare std::string with std::string
                        if (foundIngredient.getName() == ingredient_name_from_step)
                        {
                            ingredients_for_step.push_back(foundIngredient);
                            break;  // Found the ingredient, no need to check further in recipeIngredients
                        }
                    }
                }

                // FIX: Use "process" key for CookingProcess instead of "action"
                steps.push_back(CookingProcess(step["process"], ingredients_for_step));
            }

            recipes.push_back(Recipe(recipe["name"], recipe["description"], recipeIngredients, steps));
            std::cout << "Recipe Name: " << recipe["name"] << std::endl;
        }
    }
    catch (const nlohmann::json::parse_error& e)
    {
        std::cerr << "JSON parse error: " << e.what() << std::endl;
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}
