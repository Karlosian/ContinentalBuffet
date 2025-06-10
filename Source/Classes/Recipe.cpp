#include <iostream>
#include <fstream>
#include <filesystem>
#include <vector>

#include "Recipe.h"
#include "Ingredient.h"
#include "CookingProcess.h"  // Renamed from Action.h
#include "axmol.h"

#define JSON_HAS_CPP_20 0
#include "json.hpp"

std::vector<Recipe> Recipe::recipes;  // Definition for static member

// Default constructor
Recipe::Recipe() {}

// Constructor to initialize a Recipe object with name, description, ingredients, and steps
Recipe::Recipe(std::string n, std::string d, std::vector<Ingredient> i, std::vector<CookingProcess> s) {
    name              = n;
    description       = d;
    recipeIngredients = i;
    steps             = s;
}

// Getter for recipe variables
std::string Recipe::getRecipeName() {
    return name;
}

std::vector<Ingredient> Recipe::getRecipeIngredients() {
    return recipeIngredients;
}

std::vector<CookingProcess> Recipe::getRecipeSteps() {
    return steps;
}

// Function to extract all the recipes from the JSON file and store them in the static vector
void Recipe::getRecipeList() {
    std::string fullPath = ax::FileUtils::getInstance()->fullPathForFilename("recipes.json");
    std::ifstream file(fullPath);
    std::cout << "Current working directory: " << std::filesystem::current_path() << std::endl;

    if (!file.is_open())
    {
        std::cerr << "Error opening recipe file" << std::endl;
        return;
    }

    std::cout << "Reading recipes from JSON file..." << std::endl;

    try {
        nlohmann::json json_data;
        file >> json_data;

        for (const auto& recipe : json_data) {
            std::vector<Ingredient> recipeIngredients;

            for (const auto& ingredient_item : recipe["ingredients"]) {
                recipeIngredients.push_back(
                    Ingredient(ingredient_item["name"], ingredient_item["amount"], ingredient_item["unit"])
                );
            }

            std::vector<CookingProcess> steps;
            for (const auto& step : recipe["steps"]) {
                std::vector<Ingredient> ingredients_for_step;

                for (const auto& ingredient_json_value : step["ingredients"]) {
                    std::string ingredient_name_from_step = ingredient_json_value.get<std::string>();

                    for (Ingredient& foundIngredient : recipeIngredients) {
                        if (foundIngredient.getName() == ingredient_name_from_step) {
                            ingredients_for_step.push_back(foundIngredient);
                            break;
                        }
                    }
                }

                steps.push_back(CookingProcess(step["process"], ingredients_for_step));
            }

            recipes.push_back(Recipe(recipe["name"], recipe["description"], recipeIngredients, steps));
            std::cout << "Recipe Name: " << recipe["name"] << std::endl;
        }
    } catch (const nlohmann::json::parse_error& e) {
        std::cerr << "JSON parse error: " << e.what() << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}
