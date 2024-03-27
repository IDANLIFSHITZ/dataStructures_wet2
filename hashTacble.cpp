//
// Created by yuval on 3/27/2024.
//

#include "hashTable.h"

int hashTable::hashFunction(int key, int size)
{
    return key % size;
}

hashTable::~hashTable()
{
    for (int i = 0; i < maxSize; i++)
    {
        delete table[i];
    }
    delete[] table;
}

hashTable::hashTable(int size) : maxSize(size), currSize(0)
{
    table = new AVL<team*, int>*[size];
}

void hashTable::add(team* element, int key)
{
    int index = hashFunction(key, maxSize);
    table[index]->insert(element, key);
    currSize++;

    if (currSize > maxSize / 2)
    {
        resize(maxSize * 2);
    }
}

void hashTable::remove(team* element, int key)

{
    int index = hashFunction(key, maxSize);
    table[index]->remove(element, key);
    currSize--;

    if (currSize < maxSize / 4)
    {
        resize(maxSize / 2);
    }
}



void hashTable::resize(int newSize)
{
    AVL<team*, int>** newTable = new AVL<team*, int>*[newSize];

    for (int i = 0; i < maxSize; i++)
    {
        AVL<team*, int>* current = table[i];  //TODO: change tree into array

        for (int j = 0; j < current->size(); j++)
        {
            // TODO: put values and key from the array
            team* value = current->get(j);
            int key = current->getKey(j);
            int index = hashFunction(key, newSize);
            newTable[index]->insert(value, key);
        }
        delete current;
    }

    delete[] table;
    table = newTable;
    maxSize = newSize;
}

team* hashTable::find(int key)
{
    int index = hashFunction(key, maxSize);
    return table[index]->find(key);
}

int hashTable::size() const
{
    return currSize;
}

int hashTable::maximumSize() const
{
    return maxSize;
}


