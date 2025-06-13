#include <iostream>
#include <ctime>
#include <algorithm>

#include "Player.h"

// Set the player instance as a nullptr until initialized in a scene
Player* Player::instance = nullptr;
double Player::money = 100.0;  // Default money amount

// Player constructor initializes the player with default values
std::vector<Ingredient> Player::inventory; 
std::vector<bool> Player::ingredientsChosen;

// Returns the player instance (information) necessary when a new scene is loaded
Player* Player::getInstance() {
    if (instance == nullptr)
    {
        instance = new Player();
    }
    return instance;
}

// Getter for the player inventory
std::vector<Ingredient> Player::getInventory() {
    return Player::inventory;
}

// Getter for the player's "which ingredient is selected"
std::vector<bool> Player::getIngredientsChosen() {
    return Player::ingredientsChosen;
}

// Getter for player money
double Player::getMoney() {
    return Player::money;
}

// Setter for the player inventory
void Player::setIngredientsChosen(int index, bool isChosen) {
    Player::ingredientsChosen[index] = isChosen;
}

// Setter for the player's wallet
bool Player::setMoney(double i) {
    // Checks if the player has enough money to buy in the shop
    if (Player::money >= i) {
        // If so, reduce his funds by how much he spent
        Player::money -= i;
        return true;
    }
    return false;
}

// Setter for when the player gains money from dishes cooked
void Player::addMoney(double i) {
    Player::money += i;
}

// Add ingredients to the player inventory during shopping
void Player::addIngredient(Ingredient ingredient) {
    // Finds if the user already has that ingredient in their inventory (in which case it just adds to the quantity)
    std::cout << std::endl << "ASDADASD " << ingredient.getName();

    for (Ingredient& ing : inventory) {
        if (ing.getName() == ingredient.getName()) {
            ing.changeQuantityBy(ingredient.getQuantity());
            return;
        }
    }
    // Otherwise, adds a new ingredient object to the inventory
    Ingredient newIngredient = ingredient;
    inventory.push_back(newIngredient);
    ingredientsChosen.push_back(false);
}

// Using the quicksort algorithm to sort the player's inventory in alphabetical order
void Player::sortInventory(int low, int high) {
    const bool MOVING_LEFT = true, MOVING_RIGHT = false;

    // If this if-statement doesn't work, it means there is only 1 element (which doesn't need to be sorted)
    if (low < high) {
        // Setting the left and right as the two ends of the subarray
        int left = low, right = high;

        // Always starts the sort by moving from right to left
        bool currentDirection = MOVING_LEFT;

        // Sets the pivot as the first element of the subarray
        Ingredient pivot = inventory[low];

        // Runs until left and right represent the same element
        while (left < right) {
            if (currentDirection == MOVING_LEFT) {
                // Runs until it either gets to where pivot is supposed to be, or until it finds an element smaller than
                // it is
                while (pivot.getName() <= inventory[right].getName() && left < right) {
                    right--;
                }

                // When it finds that element that is smaller than it is, move it to the other side of the pivot
                inventory[left] = inventory[right];

                // Start moving from left to right
                currentDirection = MOVING_RIGHT;
            }

            if (currentDirection == MOVING_RIGHT) {
                // Runs until it either gets to where pivot is supposed to be, or until it finds an element larger than
                // it is
                while (pivot.getName() >= inventory[left].getName() && left < right) {
                    left++;
                }

                // When it finds that element that is larger than it is, move it to the other side of the pivot
                inventory[right] = inventory[left];

                // Start moving from right to left
                currentDirection = MOVING_LEFT;
            }
        }

        // Sets the pivot point at its intended position
        inventory[left] = pivot;

        // Sorts the element on the left of the pivot in the subarray
        sortInventory(low, left - 1);

        // Sorts the element on the right of the pivot in the subarray
        sortInventory(left + 1, high);
    }
}

// Overloaded method to call without need for quicksort ranges
void Player::sortInventory() {
    sortInventory(0, inventory.size() - 1);
}

// Remove a certain quantity from an ingredient
void Player::decreaseIngredientQuantity(int index, int amount) {
    inventory[index].changeQuantityBy(-amount);

    // If the quantity of the ingredient is less than or equal to 0, remove it from the inventory
    if (inventory[index].getQuantity() <= 0) {
        inventory.erase(inventory.begin() + index);
        ingredientsChosen.erase(ingredientsChosen.begin() + index);
    }
}

// Adds ten random ingredient to the player's inventory upon each playthrough
void Player::initalizeTestInventory() {
    // Gets the list of all the items in the ingredients.json file
    std::vector<Ingredient> allIngredients = Ingredient::getIngredients();

    // Get a random seed for the number generator
    srand(time(0));

    // Creates a vector to store all the ingredients seleced (to avoid having two of the same)
    std::vector<int> randomIndices;

    for (int i = 0; i < 10; i++) {
        // Find a random ingredient not yet selected
        int randomItem;
        do {randomItem = rand() % allIngredients.size();} while (std::find(randomIndices.begin(), randomIndices.end(), randomItem) != randomIndices.end());

        // Add a random quantity of this ingredient to the player's inventory
        Ingredient ingredient = allIngredients[randomItem];
        ingredient.setQuantity((int)(rand() % 10 + 1));  // Random quantity between 1 and 10
        inventory.push_back(ingredient);
        ingredientsChosen.push_back(false);

        // Add to the randomIndices vector to avoid it being chosen on the next loop
        randomIndices.push_back(randomItem);
    }

    // Sort the player's inventory
    sortInventory();
}
