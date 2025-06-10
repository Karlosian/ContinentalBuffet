#include <string>
#include <vector>
#include <iostream>

#include "CookingProcess.h"

// These are all the cooking processes that can be used in the game
std::vector<std::string> CookingProcess::CookingProcesses = {
    "Bake", "Boil", "Chop", "Fry", "Grate", "Grill", "Layer", "Mix", "Simmer", "Stir-fry", "Stuff", "Whisk"
};

CookingProcess::CookingProcess() {
    name = "";
    ingredients = std::vector<Ingredient>();
}

CookingProcess::CookingProcess(std::string n, std::vector<Ingredient> i) {
    name = n;
    ingredients = i;
}

std::string CookingProcess::getName() {
    return name;
}

std::vector<Ingredient> CookingProcess::getIngredients() {
    return ingredients;
}

std::vector<std::string> CookingProcess::getCookingProcesses() {
    return CookingProcesses;
}

void CookingProcess::setName(std::string n) {
    name = n;
}

void CookingProcess::addIngredient(Ingredient i) {
    ingredients.push_back(i);
}

void CookingProcess::removeIngredient(Ingredient i) {
    for (auto it = ingredients.begin(); it != ingredients.end(); ++it) {
        if (it->getName() == i.getName()) {
            ingredients.erase(it);
            return;
        }
    }
}

bool CookingProcess::operator==(const CookingProcess& other) const {
    if (name != other.name || CookingProcesses.size() != other.CookingProcesses.size())
    {
        return false;
    }

    for (int i = 0; i < CookingProcesses.size(); ++i)
    {
        if (CookingProcesses[i] != other.CookingProcesses[i])
            return false;
    }
    return true;
}
