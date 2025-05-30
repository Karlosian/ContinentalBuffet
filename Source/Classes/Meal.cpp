#include <vector>
#include "Meal.h"

// Parameratized Constructor
Meal::Meal(std::vector<Ingredient> i, std::vector<CookingProcess> cp) {
    recipeIngredients = i;
    steps             = cp;
    recipeAccuracy    = -1;
}

// This method searches through all the recipes stored in the JSON file and assigns a percentage
// to each recipe based on how similar it is to the user's concocted meal
Recipe Meal::findMatchingRecipe() {
    // Vector to store the score given to each recipe
    std::vector<double>recipeAccuracyArray;

    // Loop through the recipes in the JSON file
    for (int i = 0; i < recipes.size(); i++) {
        // Give a score to each recipe based on how similar it is to the ingredients used in the meal
        double ingredientScore = 0;
        int amountOfIngredients = recipes[i].getRecipeIngredients().size();

        for (int j = 0; j < amountOfIngredients; j++) {
            for (int k = 0; k < recipeIngredients.size(); k++) {
                if (recipes[i].getRecipeIngredients()[j].getName() == recipeIngredients[k].getName()) {
                    ingredientScore +=
                        1.0 / amountOfIngredients
                        * (recipes[i].getRecipeIngredients()[j].getAmount() / recipeIngredients[k].getAmount());
                    break;
                }
            }
        }
        ingredientScore *= 0.3; // Give this score a weight of 30% of the final score

        // Give a score to each recipe based on how similar it is to the cooking processes used in the meal
        double cookingProcessScore = 0;
        int minStepsSize = std::min(steps.size(), recipes[i].getRecipeSteps().size());
        for (int j = 0; j < minStepsSize; j++) {
            if (steps[j] == recipes[i].getRecipeSteps()[j]) {
                cookingProcessScore += 1.0 / recipes[i].getRecipeSteps().size();
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
    return recipes[maxIndex];
}
