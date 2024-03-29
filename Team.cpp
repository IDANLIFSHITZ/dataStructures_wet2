//
// Created by yuval on 3/27/2024.
//

#include "Team.h"

Team::Team(int id) : id(id), playersList(new LinkedList()),
                     playersTree(new AVL<Player*, Pair<int,int>()), numOfPlayers(0)
{
}


Team::~Team()
{
    delete playersList;
    playersTree->clear();
    delete playersTree;
}

StatusType Team::addPlayer(int strength)
{
    try
    {
        Player* player = new Player(numOfPlayers, strength);
        numOfPlayers++;
        playersList->push(player);
        Pair<int, int> pair(strength, player->getId());
        playersTree.insert(player, pair);
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
    int id = playersList->head->player->getId();
    playersList->pop();
    Pair<int,int> pair(player->getStrength(), player->getId()
    playersTree.remove(id);
    numOfPlayers--;
    return StatusType::SUCCESS;
}

int Team::getId() const
{
    return id;
}

int Team::getNumOfPlayers() const
{
    return numOfPlayers;
}

StatusType Team::uniteTeams(Team* other){
    playersList->uniteLists(other->playersList);

    int mySize = playersTree.getSize();
    int otherSize = other->playersTree.getSize();
    int unitedSize = mySize + otherSize;

    Pair<Player*, int>* myArrays;
    Pair<Player*, int>* otherArrays;

    Player** unitedPlayersArray;
    int* unitedKeysArray;

    try
    {
        otherArrays = other->playersTree.toArray();
    }
    catch (const std::bad_alloc& e)
    {
        return StatusType::ALLOCATION_ERROR;
    }
    try
    {
        myArrays = playersTree.toArray();

    }
    catch (const std::bad_alloc& e)
    {
        deleteArraysFromPair(otherArrays);
        delete myArrays;
        return StatusType::ALLOCATION_ERROR;
    }
    try
    {
        unitedPlayersArray = new Player*[mySize + otherSize]
    }
    catch (const std::bad_alloc& e)
    {
        deleteArraysFromPair(otherArrays);
        deleteArraysFromPair(myArrays);
        return StatusType::ALLOCATION_ERROR;
    }
    try
    {
        unitedKeysArray = new int[myArrays->getSize() + otherArrays->getSize()];
    }
    catch (const std::bad_alloc& e)
    {
        deleteArraysFromPair(otherArrays);
        deleteArraysFromPair(myArrays);
        delete unitedPlayersArray;
        return StatusType::ALLOCATION_ERROR;
    }

    // update the id of the players in "my team", starting from the size








}

StatusType deleteArraysFromPair(const Pair<Player*, int>* pair)
{
    Player** playersArray = pair->getFirst();
    int* keysArray = pair->getSecond();

    delete[] playersArray;
    delete[] keysArray;
}


StatusType mergeArrays(PLayer** arr1, int size1, Player** arr2, int size2,
                       Player** mergedPlayerArr, int sizeMerged, Pair* mergedKeysArr){
    int i = 0, j = 0, k = 0;
    while (i < size1 && j < size2){
        if (arr1[i] < arr2[j]){
            mergedPlayerArr[k] = arr1[i];
            Pair<int,int> pair(arr1[i]->getStrength(), arr1[i]->getId());
            mergedKeysArr[k] = pair;
            i++;
        } else {
            mergedArr[k] = arr2[j];
            mergedKeysArr[k] = arr2[j]->getId();
            j++;
        }
        k++;
    }
    while (i < size1){
        mergedArr[k] = arr1[i];
        Pair pair(arr1[i]->getStrength(), arr1[i]->getId());
        mergedKeysArr[k] = pair;
        i++;
        k++;
    }
    while (j < size2){
        mergedArr[k] = arr2[j];
        Pair pair(arr2[j]->getStrength(), arr2[j]->getId());
        mergedKeysArr[k] = pair;
        j++;
        k++;
    }
    return StatusType::SUCCESS;
}
