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

std::vector<bool> Player::getIngredientsChosen() {
    return Player::ingredientsChosen;
}

double Player::getMoney() {
    return Player::money;
}

// Setter for the player inventory
void Player::setIngredientsChosen(int index, bool isChosen) {
    Player::ingredientsChosen[index] = isChosen;
}

bool Player::setMoney(double i) {
    if (Player::money >= i) {
        Player::money -= i;
        return true;
    }
    else
    {
        return false;
    }
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

void Player::sortInventory() {
    sortInventory(0, inventory.size() - 1);
}

void Player::decreaseIngredientQuantity(int index, int amount) {
    inventory[index].changeQuantityBy(-amount);

    // If the quantity of the ingredient is less than or equal to 0, remove it from the inventory
    if (inventory[index].getQuantity() <= 0) {
        inventory.erase(inventory.begin() + index);
        ingredientsChosen.erase(ingredientsChosen.begin() + index);
    }
}

void Player::initalizeTestInventory() {
    std::vector<Ingredient> allIngredients = Ingredient::getIngredients();
    srand(time(0));

    std::vector<int> randomIndices;
    for (int i = 0; i < 10; i++) {
        int randomItem = rand() % allIngredients.size();

        while (std::find(randomIndices.begin(), randomIndices.end(), randomItem) != randomIndices.end()) {
            randomItem = rand() % allIngredients.size();
        }

        Ingredient ingredient = allIngredients[randomItem];
        ingredient.setQuantity((int)(rand() % 10 + 1));  // Random quantity between 1 and 10
        inventory.push_back(ingredient);
        std::cout << "Adding " << ingredient.getName() << " to inventory with indexNum " << ingredient.getNameIndex() << std::endl;
        ingredientsChosen.push_back(false);

        randomIndices.push_back(randomItem);
    }
    sortInventory();
}
