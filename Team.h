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
    AVL<Player*, int>* playersTree;
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
};


#endif // TEAM_H
