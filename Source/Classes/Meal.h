#ifndef MEAL_H_
#define MEAL_H_

#include <vector>

#include "Recipe.h"
#include "Ingredient.h"
#include "CookingProcess.h"

class Meal : public Recipe {
private:
    double recipeAccuracy;
    CookingProcess* currentStep;
public:
    // Constructors
    Meal();
    Meal(std::vector<Ingredient> i, std::vector<CookingProcess> cp);

    Recipe findMatchingRecipe();

    // Getters
    double getRecipeAccuracy();
    CookingProcess* getCurrentStep();

    // Methods
    void addStepAction(std::string action);
    void addIngredientToCurrentStep(Ingredient& i);
    void removeIngredientFromCurrentStep(Ingredient& ingredient);
    void addNewStep();

    Meal* operator=(const Meal& other);
};

#endif  // !MEAL_H_
