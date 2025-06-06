#include "Player.h"

// Set the player instance as a nullptr until initialized in a scene
Player* Player::instance = nullptr;

// Player constructor initializes the player with default values
std::vector<Ingredient*> Player::inventory; 


// Returns the player instance (information) necessary when a new scene is loaded
Player* Player::getInstance() {
    if (instance == nullptr)
    {
        instance = new Player();
    }
    return instance;
}

// Add ingredients to the player inventory during shopping
void Player::addIngredient(Ingredient *ingredient) {
    // Finds if the user already has that ingredient in their inventory (in which case it just adds to the quantity)
    for (Ingredient *ing : inventory) {
        if (ing->getName() == ingredient->getName()) {
            ing->setQuantity(ing->getQuantity() + ingredient->getQuantity());
            return;
        }
    }
    // Otherwise, adds a new ingredient object to the inventory
    inventory.push_back(ingredient);
}

void quickSort(std::vector<Ingredient*> arr, int low, int high) {
    const bool MOVING_LEFT = true, MOVING_RIGHT = false;

    // If this if-statement doesn't work, it means there is only 1 element (which doesn't need to be sorted)
    if (low < high) {
        // Setting the left and right as the two ends of the subarray
        int left = low, right = high;

        // Always starts the sort by moving from right to left
        bool currentDirection = MOVING_LEFT;

        // Sets the pivot as the first element of the subarray
        Ingredient* pivot      = arr[low];

        // Runs until left and right represent the same element
        while (left < right) {
            if (currentDirection == MOVING_LEFT) {
                // Runs until it either gets to where pivot is supposed to be, or until it finds an element smaller than it is
                while (pivot->getName() < arr[right]->getName() && left < right) {
                    right--;
                }

                // When it finds that element that is smaller than it is, move it to the other side of the pivot
                arr[left] = arr[right];

                // Start moving from left to right
                currentDirection = MOVING_RIGHT;
            }

            if (currentDirection == MOVING_RIGHT) {
                // Runs until it either gets to where pivot is supposed to be, or until it finds an element larger than it is
                while (pivot->getName() > arr[left]->getName() && left < right) {
                    left++;
                }

                // When it finds that element that is larger than it is, move it to the other side of the pivot
                arr[right] = arr[left];

                // Start moving from right to left
                currentDirection = MOVING_LEFT;
            }
        }

        // Sets the pivot point at its intended position
        arr[left] = pivot;

        // Sorts the element on the left of the pivot in the subarray
        quickSort(arr, low, left - 1);

        // Sorts the element on the right of the pivot in the subarray
        quickSort(arr, left + 1, high);
    }
}

void Player::sortInventory() {
    quickSort(inventory, 0, inventory.size() - 1);
}
