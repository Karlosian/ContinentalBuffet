#include <vector>
#include <iostream>

#include "Meal.h"
#include "Player.h"

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
    for (int i = 0; i < Recipe::getAllRecipes().size(); i++) {
        // Give a score to each recipe based on how similar it is to the ingredients used in the meal
        double ingredientScore = 0;
        int amountOfIngredients = Recipe::getAllRecipes()[i].getRecipeIngredients().size();

        for (int j = 0; j < amountOfIngredients; j++) {
            for (int k = 0; k < this->recipeIngredients.size(); k++) {
                if (Recipe::getAllRecipes()[i].getRecipeIngredients()[j].getName() == this->recipeIngredients[k].getName()) {
                    ingredientScore +=
                        1.0 / amountOfIngredients
                        * (Recipe::getAllRecipes()[i].getRecipeIngredients()[j].getAmount() / this->recipeIngredients[k].getAmount());
                    break;
                }
            }
        }
        ingredientScore *= 0.3; // Give this score a weight of 30% of the final score

        // Give a score to each recipe based on how similar it is to the cooking processes used in the meal
        double cookingProcessScore = 0;
        int minStepsSize = std::min(steps.size(), Recipe::getAllRecipes()[i].getRecipeSteps().size());
        for (int j = 0; j < minStepsSize; j++) {
            if (this->steps[j] == Recipe::getAllRecipes()[i].getRecipeSteps()[j]) {
                cookingProcessScore += 1.0 / Recipe::getAllRecipes()[i].getRecipeSteps().size();
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
    std::vector<Recipe> test = Recipe::getAllRecipes();
    return Recipe::getAllRecipes()[maxIndex];
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

// Goes through the actions the player took to make the dish and substract the ingredients used from their inventory
void Meal::substractUsedIngredients() {
    // Vector to keep track of which ingredient was used
    std::vector<std::string> ingredientsReduced;

    for (CookingProcess stepsTaken : steps) {
        for (Ingredient ingredientInStep : stepsTaken.getIngredients()) {
            std::vector<Ingredient> playerInventory = Player::getInventory();
            for (int i = 0; i < playerInventory.size(); i++) {
                // Avoids having the same ingredient removed multiple times from the player's inventory
                bool hasBeenReduced = false;

                for (std::string ing : ingredientsReduced) {
                    if (playerInventory[i].getName() == ing) {
                        hasBeenReduced = true;
                    }
                }

                // Reduce quantity of the ingredient
                if (playerInventory[i].getName() == ingredientInStep.getName() && !hasBeenReduced) {
                    Player::decreaseIngredientQuantity(i, 1);
                    ingredientsReduced.push_back(ingredientInStep.getName());
                    break;
                }
            }
        }
    }
}

// Continue to next cooking step
void Meal::addNewStep() {
    if (currentStep != nullptr) {
        steps.push_back(*currentStep);
    }
    currentStep = new CookingProcess();
}
