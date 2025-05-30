#include "Player.h"

// Set the player instance as a nullptr until initialized in a scene
Player* Player::instance = nullptr;

// Returns the player instance (information) necessary when a new scene is loaded
Player* Player::getInstance() {
    if (instance == nullptr)
    {
        instance = new Player();
    }
    return instance;
}

/* Add ingredients to the player inventory during shopping
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
}*/
