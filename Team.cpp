//
// Created by yuval on 3/27/2024.
//

#include "Team.h"

Team::Team(int id) : id(id), playersList(new LinkedList()),
                     playersTree(new AVL<Player*, int>()), numOfPlayers(0)
{
}


Team::~Team()
{
    delete playersList;
    delete playersTree;
}

StatusType Team::addPlayer(int strength)
{
    try
    {
        Player* player = new Player(numOfPlayers, strength);
        numOfPlayers++;
        playersList->add(player);
        playersTree.insert(player, player->getId());
    }
    catch (const std::bad_alloc& e)
    {
        return StatusType::ALLOCATION_ERROR;
    }
    return StatusType::SUCCESS;
}

StatusType Team::removeNewestPlayer()
{
    if (numOfPlayers == 0)
    {
        return StatusType::FAILURE;
    }
    int id = playerList->head->getId();
    playersList->pop();
    playersTree.remove(id);
    numOfPlayers--;
    return StatusType::SUCCESS;
}
