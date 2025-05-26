#include <vector>
#include "Meal.h"

Recipe Meal::findMatchingRecipe() {
    int *recipeAccuracyArray = new int[recipes.size()];

    // Give a score to each recipe based on how similar it is to the ingredients and cooking process of the meal
    for (int i = 0; i < recipes.size(); i++) {
        double ingredientScore = 0;
        int amountOfIngredients = recipes[i].getIngredients().size();

        for (int j = 0; j < amountOfIngredients; j++) {
            for (int k = 0; k < ingredients.size(); k++) {
                if (recipes[i].getIngredients()[j].getName() == ingredients[k].getName()) {
                    ingredientScore +=
                        1.0 / amountOfIngredients
                        * (recipes[i].getIngredients()[j].getAmount() / ingredients[k].getAmount());
                    break;
                }
            }
        }

        ingredientScore *= 0.3;

        double cookingProcessScore = 0;
        for (int j = 0; j < recipes[i].getCookingProcess().size(); j++) {
            if (steps[j] == recipes[i].getCookingProcess()[j]) {
                cookingProcessScore += 1.0 / recipes[i].getCookingProcess().size();
            }
        }
        cookingProcessScore *= 0.7;
        recipeAccuracyArray[i] = ingredientScore + cookingProcessScore;
    }

    // Find the recipe with the highest score
    int maxIndex = 0;
    for (int i = 1; i < recipes.size(); i++) {
        if (recipeAccuracy[i] > recipeAccuracy[maxIndex]) {
            maxIndex = i;
        }
    }

    // Return the recipe with the highest score
    recipeAccuracy = recipeAccuracyArray[maxIndex];
    delete[] recipeAccuracyArray;
    return recipes[maxIndex];
}
