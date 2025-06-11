#include <vector>
#include <iostream>

#include "Meal.h"

// Default Constructor
Meal::Meal() {
    recipeAccuracy = -1;
    currentStep    = new CookingProcess();
}

// Parameratized Constructor
Meal::Meal(std::vector<Ingredient> i, std::vector<CookingProcess> cp) {
    recipeIngredients = i;
    steps             = cp;
    recipeAccuracy    = -1;
    currentStep       = new CookingProcess();
}

// Getters
CookingProcess* Meal::getCurrentStep() {
    return currentStep;
}

double Meal::getRecipeAccuracy() {
    return recipeAccuracy;
}

// This method searches through all the recipes stored in the JSON file and assigns a percentage
// to each recipe based on how similar it is to the user's concocted meal
Recipe Meal::findMatchingRecipe() {
    // Vector to store the score given to each recipe
    std::vector<double>recipeAccuracyArray;

    // Loop through the recipes in the JSON file
    for (int i = 0; i < Recipe::recipes.size(); i++) {
        // Give a score to each recipe based on how similar it is to the ingredients used in the meal
        double ingredientScore = 0;
        int amountOfIngredients = Recipe::recipes[i].getRecipeIngredients().size();

        for (int j = 0; j < amountOfIngredients; j++) {
            for (int k = 0; k < this->recipeIngredients.size(); k++) {
                if (Recipe::recipes[i].getRecipeIngredients()[j].getName() == this->recipeIngredients[k].getName()) {
                    ingredientScore +=
                        1.0 / amountOfIngredients
                        * (Recipe::recipes[i].getRecipeIngredients()[j].getAmount() / this->recipeIngredients[k].getAmount());
                    break;
                }
            }
        }
        ingredientScore *= 0.3; // Give this score a weight of 30% of the final score

        // Give a score to each recipe based on how similar it is to the cooking processes used in the meal
        double cookingProcessScore = 0;
        int minStepsSize = std::min(steps.size(), recipes[i].getRecipeSteps().size());
        for (int j = 0; j < minStepsSize; j++) {
            if (this->steps[j] == Recipe::recipes[i].getRecipeSteps()[j]) {
                cookingProcessScore += 1.0 / Recipe::recipes[i].getRecipeSteps().size();
            }
        }
        cookingProcessScore *= 0.7; // Give this score a weight of 70% of the final score

        // Add the score to the vector array (at the same index as the recipe in the recipes vector)
        recipeAccuracyArray.push_back(ingredientScore + cookingProcessScore);
    }

    // Find the recipe with the highest score
    int maxIndex = 0;
    for (int i = 1; i < recipeAccuracyArray.size(); i++) {
        if (recipeAccuracyArray[i] > recipeAccuracyArray[maxIndex]) {
            maxIndex = i;
        }
    }

    // Return the recipe with the highest score
    recipeAccuracy = recipeAccuracyArray[maxIndex];
    std::vector<Recipe> test = Recipe::recipes;
    return Recipe::recipes[maxIndex];
}

// Overloaded Assignment Operator
Meal* Meal::operator=(const Meal& other) {
    if (this != &other) {
        Recipe::operator=(other);
        recipeAccuracy = other.recipeAccuracy;
    }
    return this;
}

// Add the step to the meal
void Meal::addStepAction(std::string action) {
    currentStep->setName(action);
}

// Add an ingredient to current cooking step
void Meal::addIngredientToCurrentStep(Ingredient &ingredient) {
    currentStep->addIngredient(ingredient);
}

// Remove an ingredient from current cooking step
void Meal::removeIngredientFromCurrentStep(Ingredient &ingredient) {
    currentStep->removeIngredient(ingredient);
}

// Continue to next cooking step
void Meal::addNewStep() {
    if (currentStep != nullptr) {
        steps.push_back(*currentStep);
    }
    currentStep = new CookingProcess();
}
