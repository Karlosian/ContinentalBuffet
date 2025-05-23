#include <vector>
#include <fstream>
#include <iostream>
#include <filesystem>

#include "Ingredient.h"
#define JSON_HAS_CPP_20 0
#include "json.hpp"
#include "axmol.h"

std::vector<Ingredient> Ingredient::ingredients;

Ingredient::Ingredient() {name = ""; cost = 0; quantity = 0;}

Ingredient::Ingredient(std::string n, int a, std::string u) {
    name = n;
    amount = a;
    unit = u;
    cost = 0;
    quantity = 0;
}

Ingredient::Ingredient(std::string n, std::string u, double c, int a, int t) {
    name = n;
    cost = c;
    amount = a;
    type = t;
    unit = u;
}

std::string Ingredient::getName() {
    return name;
}

double Ingredient::getCost() {
    return cost;
    
}

int Ingredient::getQuantity() {
    return quantity;
}

std::vector<Ingredient> Ingredient::getIngredients() {
    return ingredients;
}

void Ingredient::setQuantity(int q) {
    quantity = q;
}

void Ingredient::changeQuantityBy(int q) {
    quantity += q;
}

void Ingredient::getIngredientList() {
    std::ifstream file("ingredients.json");
    std::cout << "Current working directory: " << std::filesystem::current_path() << std::endl;

    if (!file.is_open()) {
        std::cerr << "Error opening file" << std::endl;
        return;
    }

    std::cout << "Reading ingredients from JSON file..." << std::endl;

    try {
        nlohmann::json json_data;
        file >> json_data;

        for (const auto& item : json_data) {
            Ingredient ingredient(item["name"], item["unit"], item["cost"], item["amount"], item["type"]);
            Ingredient::ingredients.push_back(ingredient);

            // Debugging purposes
            std::cout << "Ingredient: " << ingredient.name << ", Cost: " << ingredient.cost
                      << ", Amount: " << ingredient.amount << " " << ingredient.unit << ", Type: " << ingredient.type
                      << std::endl;
        }
    } catch (const nlohmann::json::parse_error& e) {
        std::cerr << "JSON parse error: " << e.what() << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}
