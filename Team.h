//
// Created by yuval on 3/27/2024.
//

#ifndef TEAM_H
#define TEAM_H
#include "Player.h"
#include "LinkedList.h"
#include "AVL.h"

class Team {
private:
    int id;
    LinkedList* playersList;
    AVL<Player*, Pair<int,int>>* playersTree;
    int numOfPlayers;
public:

    Team() = default;

    explicit Team(int id);

    /**
     * the big three- copy constructor, destructor and assignment operator
     */
    ~Team();
    Team(const Team& other) = delete;
    Team& operator=(const Team& other) = delete;

    /**
     * public functions: addPlayer, removeNewestPlayer
     * getId, getNumOfPlayers, getPlayer
     */
    StatusType addPlayer(int strength);
    StatusType removeNewestPlayer();

    int getId() const;
    int getNumOfPlayers() const;
    Player* getPlayer(int id) const;

    StatusType uniteTeams(Team* other);
};

StatusType deleteArraysFromPair(const Pair<Player*, int>* pair);

template <class T>
StatusType mergeArrays(T* arr1, int size1, T* arr2, int size2, T* mergedArr, int sizeMerged){
    int i = 0, j = 0, k = 0;
    while (i < size1 && j < size2){
        if (arr1[i] < arr2[j]){
            mergedArr[k] = arr1[i];
            i++;
        } else {
            mergedArr[k] = arr2[j];
            j++;
        }
        k++;
    }
    while (i < size1){
        mergedArr[k] = arr1[i];
        i++;
        k++;
    }
    while (j < size2){
        mergedArr[k] = arr2[j];
        j++;
        k++;
    }
    return StatusType::SUCCESS;
}

#endif // TEAM_H
