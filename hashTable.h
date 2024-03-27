//
// Created by yuval on 3/27/2024.
//

#ifndef HASHTABLE_H
#define HASHTABLE_H



// start of class HashTable
// a dynamic array of avl trees, with one hash function (modulus of the key by the size of the array)
// the hash table will resize itself when it is more than 50% full or less than 25% full
class hashTable {
private:
    AVL<team*, int>* table[];
    int currSize;
    int maxSize;

    public:

    /**
     * the hash function to use
     * @param key
     * @param size
     * @return the index in the hash table
     */
    static int hashFunction(int key, int size);



    /*
     * public functions:
     * basic constructor (size, hash function)
     * copy constructor, destructors and assignment operators
     * add, remove, find
     * size, maxSize
     */

    /**
     * constructor for the hash table
     * @param size the size of the hash table
     */
    explicit hashTable(int size);

    /**
     * the big three- copy constructor, destructor and assignment operator
     */


    /** copy constructor for the hash table
     * @param other
     */
    hashTable(const hashTable& other) = delete;

    /** destructor for the hash table
     */
    ~hashTable();

    /** add a new element to the hash table
     * @param element
     * @param key
     * @note if the table is more than 50% full, resize it
     */
    void add(team* element, int key);

    /** remove an element from the hash table
     * @param element
     * @param key
     * @note if the table is less than 25% full, resize it
     */
    void remove(team* element, int key);

    /** resize the hash table
     * @param newSize
     */
    void resize(int newSize);

    /** find an element in the hash table
     * @param key
     * @return the element if found, nullptr otherwise
     */
    team* find(int key);

    /** get the current size of the hash table
     * @return the current size of the hash table
     */
    int size() const;

    /** get the maximum size of the hash table
     * @return the maximum size of the hash table
     */
    int maximumSize() const;
};


#endif //HASHTABLE_H
