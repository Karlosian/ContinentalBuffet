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

Ingredient::Ingredient(std::string n, std::string u, double c, int a, int t) {
    name = n;
    cost = c;
    amount = a;
    type = t;
}

double Ingredient::getCost() {
    return cost;
    
}

int Ingredient::getQuantity() {
    return quantity;
}

void Ingredient::setQuantity(int q) {
    quantity = q;
}

void Ingredient::changeQuantityBy(int q) {
    quantity += q;
}

void Ingredient::getIngredients() {
    std::ifstream file("ingredients.json");

    if (!file.is_open()) {
        std::cerr << "Error opening file" << std::endl;
        return;
    }

    try {
        nlohmann::json json_data;
        file >> json_data;

        for (const auto& item : json_data) {
            Ingredient ingredient(item["name"], item["unit"], item["cost"], item["amount"], item["type"]);
            Ingredient::ingredients.push_back(ingredient);
        }
    } catch (const nlohmann::json::parse_error& e) {
        std::cerr << "JSON parse error: " << e.what() << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}
