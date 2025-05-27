#include "Player.h"

Player* Player::instance = nullptr;

Player* Player::getInstance() {
    if (instance == nullptr)
    {
        instance = new Player();
    }
    return instance;
}
