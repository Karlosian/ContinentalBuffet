#include "Ingredient.h"

int Ingredient::getCost() {
    return cost;
}

int Ingredient::getQuantity() {
    return quantity;
}

void Ingredient::setQuantity(int q) {
    quantity = q;
}

void Ingredient::changeQuantityBy(int q) {
    quantity += q;
}
