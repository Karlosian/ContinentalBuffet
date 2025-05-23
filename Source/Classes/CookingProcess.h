#ifndef COOKINGPROCESS_H_
#define COOKINGPROCESS_H_

#include <string>
#include <vector>

#include "Ingredient.h"

class CookingProcess {
private:
    std::string name;
    std::vector<Ingredient> ingredients;

    static std::string process;
public:
    CookingProcess(std::string n, std::vector<Ingredient> i);
};

#endif  // !COOKINGPROCESS_H_
