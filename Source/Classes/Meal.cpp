#include <vector>
#include "Meal.h"

Meal::Meal(std::vector<Ingredient> i, std::vector<CookingProcess> cp) {
    recipeIngredients = i;
    steps = cp;
}

Recipe Meal::findMatchingRecipe() {
    std::vector<double>recipeAccuracyArray;

    // Give a score to each recipe based on how similar it is to the ingredients and cooking process of the meal
    for (int i = 0; i < recipes.size(); i++) {
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

        ingredientScore *= 0.3;

        double cookingProcessScore = 0;
        int minStepsSize = std::min(steps.size(), recipes[i].getRecipeSteps().size());
        for (int j = 0; j < minStepsSize; j++) {
            if (steps[j] == recipes[i].getRecipeSteps()[j]) {
                cookingProcessScore += 1.0 / recipes[i].getRecipeSteps().size();
            }
        }
        cookingProcessScore *= 0.7;
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
