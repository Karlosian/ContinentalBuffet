#ifndef COOKINGPROCESS_H_
#define COOKINGPROCESS_H_

#include <string>
#include <vector>

#include "Ingredient.h"

// Class to store the cooking process (what action to take with what ingredients)
class CookingProcess {
private:
    // The name of the cooking process
    std::string name;

    // The ingredients needed for the cooking process
    std::vector<Ingredient> ingredients;

    // Every cooking process is stored in a static vector
    static std::vector<std::string> CookingProcesses;
public:
    // Default constructor
    CookingProcess(std::string n, std::vector<Ingredient> i);

    bool operator==(const CookingProcess& other) const;
};
#endif  // !COOKINGPROCESS_H_
