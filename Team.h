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
    LinkedList* players;
    AVL<Player*, int> playersTree;
    int numOfPlayers;
public:

    Team() = default;

    explicit Team(int id);

    ~Team();
    Team(const Team& other) = delete;
    Team& operator=(const Team& other) = delete;

    void addPlayer(Player* player);
    void removeNewestPlayer(Player* player);

    int getId() const;
    int getNumOfPlayers() const;
    Player* getPlayer(int id) const;
};


#endif // TEAM_H
