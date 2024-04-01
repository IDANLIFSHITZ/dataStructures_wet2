//
// Created by yuval on 4/1/2024.
//
#include "hashTable.h"
#include "Team.h"
#include <iostream>
using namespace std;
int main(){
hashTable table;
Team team1(1);
Team team2(2);
table.add(&team1, 1);
table.add(&team2, 2);
Team* t = table.find(1).ans();
    if (&team1 == t)
        cout << "team1 found" << endl;
    else
        cout << "team1 not found" << endl;

    cout << "size: " << table.getSize() << endl;
    table.remove(1);
    cout << "size: " << table.getSize() << endl;
    table.remove(2);
    cout << "size: " << table.getSize() << endl;
}
