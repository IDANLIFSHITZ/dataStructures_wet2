//
// Created by yuval on 3/27/2024.
//

#include "Team.h"

Team::Team(int id) : id(id), players(new LinkedList()), playersTree(new AVL<Player*, int>())
{
}


Team::~Team()
{
    delete players;
    delete playersTree;
}

void Team::addPlayer(Player* player)
{
    players->add(player);
    playersTree.insert(player, player->getId());
}

void Team::removeNewestPlayer(Player* player)
{
    int id = players->removeNewest(player);
    playersTree.remove(player, player->getId());
}
