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
