//
// Created by yuval on 3/27/2024.
//

#ifndef WET24_TO_PUBLISH_LINKEDLIST_H
#define WET24_TO_PUBLISH_LINKEDLIST_H

#include "wet2util.h"

class LinkedList {
private:
    class Node {
    public:
        int data;
        Node* next;
        Node(int data, Node* next) : data(data), next(next) {}
        ~Node() = default;
    };
    Node* head;
    int size;
public:
    // constructor
    LinkedList();
    // destructor
    ~LinkedList();
    // copy constructor
    LinkedList(const LinkedList& other) = delete;
    // assignment operator
    LinkedList& operator=(const LinkedList& other) = delete;
    // push
    StatusType push(int data);
    // pop
    StatusType pop();
    // get size
    int getSize() const;
};


#endif //WET24_TO_PUBLISH_LINKEDLIST_H
