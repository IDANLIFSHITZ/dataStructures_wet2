//
// Created by yuval on 3/27/2024.
//

#include "LinkedList.h"
#include "wet2util.h"

LinkedList::LinkedList() : head(nullptr), size(0)
{
}

LinkedList::~LinkedList()
{
    Node* current = head;
    while (current != nullptr)
    {
        Node* temp = current->next;
        delete current;
        current = temp;
    }
}

StatusType LinkedList::push(int data){
    try
    {
        Node* newNode = new Node(data, head);
        head = newNode;
        size++;
    }
    catch (const std::bad_alloc& e)
    {
        return StatusType::ALLOCATION_ERROR;
    }
    return StatusType::SUCCESS;
}

StatusType LinkedList::pop()
{
    if (head == nullptr)
    {
        return StatusType::FAILURE;
    }
    Node* temp = head;
    head = head->next;
    delete temp;
    size--;
    return StatusType::SUCCESS;
}

int LinkedList::getSize() const
{
    return size;
}