#include "CookingProcess.h"

/*std::string Process::Processs[] = {
    // Cooking Methods
    "Boil", "Fry", "Bake", "Grill", "Stir-fry", "Stimmer",
    // Prep Processs
    "Chop", "Mix", "Whisk", "Grate", "Layer", "Stuff"
};*/

CookingProcess::CookingProcess(std::string n, std::vector<Ingredient> i) {
    name = n;
    ingredients = i;
}
