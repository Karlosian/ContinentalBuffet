#ifndef PLAYER_H_
#define PLAYER_H_

#include <vector>

#include "Ingredient.h"

class Player {
private:
    Player();
    static Player* instance;
public:
    static Player* getInstance();
    std::vector<Ingredient*> inventory;
};

#endif
