#include <string>
#include <vector>

#include "CookingProcess.h"

// These are all the cooking processes that can be used in the game
std::vector<std::string> CookingProcess::CookingProcesses = {
    // Cooking Methods
    "Boil", "Fry", "Bake", "Grill", "Stir-fry", "Stimmer",
    // Prep Processs
    "Chop", "Mix", "Whisk", "Grate", "Layer", "Stuff"
};

CookingProcess::CookingProcess(std::string n, std::vector<Ingredient> i) {
    name = n;
    ingredients = i;
}

bool CookingProcess::operator==(const CookingProcess& other) const {
    if (name != other.name || CookingProcesses.size() != other.CookingProcesses.size()) {
        return false;
    }

    for (int i = 0; i < CookingProcesses.size(); ++i) {
        if (CookingProcesses[i] != other.CookingProcesses[i]) return false;
    }
    return true;
}
