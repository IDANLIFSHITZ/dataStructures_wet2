//
// Created by yuval on 3/27/2024.
//

#include "Player.h"

Player::Player(int id, int strength) : id(id), strength(strength)
{
}

int Player::getId() const
{
    return id;
}

int Player::getStrength() const
{
    return strength;
}
