//
// Created by magshimim on 3/27/2024.
//

#include <iostream>
#include "wet2util.h"

#ifndef WET24_TO_PUBLISH_AVL_H
#define WET24_TO_PUBLISH_AVL_H

template<class valT, class keyT>
class AVL {
private:
    class alreadyInTree : public std::exception {
    };

    class notInTree : public std::exception {
    };

    class Node {
    public:
        /*
         * Node members.
         */
        keyT key;
        valT data;
        int height; //height of node (null is -1).
        int subtreeSize;
        Node* parent;
        Node* left; //left child.
        Node* right; //right child.
        int extra;
        int maxScoreInSubtree;


        explicit Node(valT data, const keyT& key, Node* parent = nullptr) :
            key(key),
            data(data),
            height(0),
            subtreeSize(0),
            parent(parent),
            left(nullptr),
            right(nullptr),
            extra(0),
            maxScoreInSubtree(0)
            {}

        ~Node()
        {
            delete this->left;
            delete this->right;
            return;
        }

        /*
         * operators on Node class:
         */
        bool operator==(const Node &node) {
            return (this->key == node.key);
        }

        bool operator!=(const Node &node) {
            return (this->key != node.key);
        }

        bool operator<(const Node &node) {
            return (this->key < node.key);
        }

        bool operator>(const Node &node) {
            return (this->key > node.key);
        }
        ///////////////////////////////////////
        /*
        * returns height of node.
        */
        static int get_height(Node *node) {
            if (node == nullptr) {
                return -1;
            }
            return node->height;
        }

        /*
         * calculates and returns balance factor.
         */
        int get_balance() const {
            if (this->height == 0) {
                return 0;
            }
            return (get_height(this->left) - get_height(this->right));
        }

        /*
         * updates height of Node;
         */
        void update_height() {
            this->height = max(get_height(this->left), get_height(this->right)) + 1;
        }

        /*
         * removes node from tree and updates parents and children.
         */
        /*
        void remove()
        {
            if (this->left == nullptr && this->right == nullptr) // if node has no children.
            {
                if (parent->left == this) //if left child of parent.
                {
                    parent->left = nullptr;
                }
                else
                {
                    parent->right = nullptr;
                }
            }
        }
         */
    };

    Node *root; //root of AVL tree.
    int numOfNodes;

    Node *minNode; //ptr to smallest node in the tree.
    Node *maxNode; //ptr to biggest node in the tree.

    /*
    * returns Node with key=key if exists, else return nullptr.
    */
    Node *search_aux(Node *currNode, const keyT& key) const {
        if (currNode == nullptr) //if in empty leaf.
        {
            return nullptr;
        } else if (currNode->key > key) //if key is in left subtree.
        {
            return search_aux(currNode->left, key);
        } else if (currNode->key < key) //if key is in right subtree.
        {
            return search_aux(currNode->right, key);
        }
        return currNode; //currNode is the node to find.
    }

    /*
     * executes right rotate. returns original left node.
     */
    Node *right_rotate(Node *nodeToRotate, int currExtra)
    {
        Node* leftNode = nodeToRotate->left;
        int nodeToRotateExtra = nodeToRotate->extra; // save extra of nodeToRotate.
        int leftNodeExtra = leftNode->extra; // save extra of nodeToRotate->left.
        nodeToRotate->extra = leftNodeExtra;
        leftNode->extra += nodeToRotateExtra;
        //rotate.

        nodeToRotate->left = leftNode->right;
        if (leftNode->right != nullptr)
        {
            leftNode->right->extra += leftNodeExtra;
            leftNode->right->parent = nodeToRotate;
        }

        leftNode->right = nodeToRotate;
        leftNode->parent = nodeToRotate->parent;
        nodeToRotate->parent = leftNode;

        if (leftNode->parent != nullptr && nodeToRotate->key < leftNode->parent->key) // if org nodeToRotate wasn't root.
        { // needs to update org parent children.
            leftNode->parent->left = leftNode;
        }
        else
        {
            if (leftNode->parent != nullptr)
            {
                leftNode->parent->right = leftNode;
            }
        }

        nodeToRotate = leftNode;

        //update heights.
        nodeToRotate->update_height();
        if (nodeToRotate->right != nullptr)
        {
            nodeToRotate->right->update_height();
        }
        if (nodeToRotate->left != nullptr)
        {
            nodeToRotate->left->update_height();
        }
        leftNode->update_height();

        return nodeToRotate;
    }

    /*
     * executes right rotate. returns original right node.
     */
    Node *left_rotate(Node *nodeToRotate) {
        Node *rightNode = nodeToRotate->right;
        Node *leftNodeOfRightNode = rightNode->left;

        //rotate.
        rightNode->left = nodeToRotate;
        nodeToRotate->right = leftNodeOfRightNode;

        //update heights.
        nodeToRotate->update_height();
        rightNode->update_height();

        return rightNode;
    }

    /*
     * adds a new node to the tree recursively.
     * returns the node that in supposed to be the current node in the recursion after insertion and rotations.
     */
    Node *insert_aux(Node *currNode, Node *newNode) {
        /* part 1: normal tree insertion. */

        if (currNode == nullptr) //if arrived at leaf.
        {
            return newNode;
        }

        if (*newNode < *currNode) //if newNode belong in left subtree.
        {
            currNode->left = insert_aux(currNode->left, newNode);
        } else if (*newNode > *currNode) //if newNode belong in right subtree.
        {
            currNode->right = insert_aux(currNode->right, newNode);
        } else // newNode already in AVL tree.
        {
            throw alreadyInTree();
        }

        /* part 2: update height */

        currNode->update_height();

        /* part 3: calc balance factor and perform rotations if tree  is unbalanced */

        int balanceFactor = currNode->get_balance();
        //LL rotate.
        if (balanceFactor > 1 && *newNode < *currNode->left) {
            currNode = right_rotate(currNode);
        }
            //RR rotate.
        else if (balanceFactor < -1 && *newNode > *currNode->right) {
            currNode = left_rotate(currNode);
        }
            //LR rotate.
        else if (balanceFactor > 1 && *newNode > *currNode->left) {
            currNode->left = left_rotate(currNode->left);
            currNode = right_rotate(currNode);
        }
            //RL rotate.
        else if (balanceFactor < -1 && *newNode < *currNode->right) {
            currNode->right = right_rotate(currNode->right);
            currNode = left_rotate(currNode);
        }
        return currNode;
    }

    /*
     * returns smallest node in tree with root of currNode.
     * assumes tree is not empty.
     */
    Node* get_min_node(Node *node) {
        while (node->left != nullptr) //while there is a left child.
        {
            node = node->left;
        }
        return node;
    }

    /*
     * returns smallest node in tree with root of currNode.
     * assumes tree is has at least two nodes and if currNode is minNode of subtree returns him.
     */
    Node* get_father_of_min_node(Node* currNode, Node* miniNode)
    {
        if (currNode == miniNode)
        {
            return currNode;
        }
        while (currNode->left != miniNode) //while there is a left child.
        {
            currNode = currNode->left;
        }
        return currNode;
    }

    /*
     * returns biggest node in tree with root of currNode.
     * assumes tree is not empty.
     */
    Node *get_max_node(Node *node) {
        while (node->right != nullptr) //while there is a right child.
        {
            node = node->right;
        }
        return node;
    }

    /*
    * removes a node from the tree recursively.
    * returns the node that in supposed to be the current node in the recursion after deletion and rotations.
    */
    Node *remove_aux(Node *currNode, keyT keyToDelete) {
        /* part 1: find and delete the node to be deleted */

        if (currNode == nullptr) //if arrived at leaf and key not found.
        {
            throw notInTree();
        }

        if (currNode->key > keyToDelete) //if key is in left subtree.
        {
            currNode->left = remove_aux(currNode->left, keyToDelete);
        }
        else if (currNode->key < keyToDelete) //if key is in right subtree.
        {
            currNode->right = remove_aux(currNode->right, keyToDelete);
        }
        else //the current node is the node to be deleted;
        {
            if (currNode->right == nullptr ||
                currNode->left == nullptr) //if current node has only one child or no children.
            {
                Node *temp = currNode->left ? currNode->left
                                            : currNode->right; //if has left child: temp = left child, else temp = right child.
                delete currNode; // deletes old node.
                currNode = temp; //currNode = son if exists, if not nullptr.
            }
            else //currNode has two children.
            {
                Node* nextBiggest = get_min_node(currNode->right); //nextBiggest is the smallest node that is bigger then currNode.
                Node* nextBiggestParent = get_father_of_min_node(currNode->right, nextBiggest);

                Node* node2Delete = currNode;
                Node* node2DeleteLeft = currNode->left;
                Node* node2DeleteRight = currNode->right;
                node2Delete->right = nextBiggest->right;
                node2Delete->left = nextBiggest->left; //nextBiggest->left = nullptr (always).

                currNode = nextBiggest;
                currNode->left = node2DeleteLeft;
                currNode->right = node2DeleteRight;
                if (nextBiggest == nextBiggestParent) //if next biggest is currNode->right.
                {
                    currNode->right = node2Delete;
                }
                else //if next biggest is not currNode->right (somewhere in subtree.).
                {
                    nextBiggestParent->left = node2Delete;
                }
                currNode->right = remove_aux(currNode->right, keyToDelete);
            }
        }

        if (currNode == nullptr)//if the subtree of currNode had only one node.
        {
            return currNode;
        }

        /* part 2: update height */

        currNode->update_height();

        /* part 3: calc balance factor and perform rotations if tree  is unbalanced */

        int balanceFactor = currNode->get_balance();
        //LL rotate.
        if (balanceFactor > 1 && currNode->left->get_balance() >= 0)
        {
            currNode = right_rotate(currNode);
        }
            //RR rotate.
        else if (balanceFactor < -1 && currNode->right->get_balance() <= 0)
        {
            currNode = left_rotate(currNode);
        }
            //LR rotate.
        else if (balanceFactor > 1 && currNode->left->get_balance() < 0)
        {
            currNode->left = left_rotate(currNode->left);
            currNode = right_rotate(currNode);
        }
            //RL rotate.
        else if (balanceFactor < -1 && currNode->right->get_balance() > 0)
        {
            currNode->right = right_rotate(currNode->right);
            currNode = left_rotate(currNode);
        }

        return currNode;
    }

    /*
     * adds current node to Array at index and adds subtrees.
     * returns updated index after all insertions.
     */
    /*
    int tree_to_sorted_array(Pair<keyT, valT>* array, int index, Node *currNode) const {
        if (currNode == nullptr) {
            return index;
        }
        index = tree_to_sorted_array(array, index, currNode->left); //adds left subtree to Array.
        Pair<keyT, valT> pair(currNode->key, currNode->data);
        array[index++] = pair;
        index = tree_to_sorted_array(array, index, currNode->right); //adds right subtree to Array.
        return index;
    }
    */

    /*
     * builds tree from sorted array, returns root of recursive tree.
     */
    /*
    Node* sorted_array_to_tree(Pair<keyT, valT> *array, int start, int end) {
        if (start > end) {
            return nullptr;
        }

        int mid = (start + end) / 2;
        Node *newRoot = new Node(array[mid].get_second(), array[mid].get_first()); //create new root of recursive tree.

        newRoot->left = sorted_array_to_tree(array, start, mid - 1); //create left subtree recursively.
        newRoot->right = sorted_array_to_tree(array, mid + 1, end); //create right subtree recursively.

        return newRoot;
    }
    */

    /*
     * clears all nodes in the tree with root of currRoot recursively.
     */
    void clear_aux(Node *currRoot) {
        if (currRoot == nullptr) {
            return;
        }
        clear_aux(currRoot->left); //clears left subtree.
        clear_aux(currRoot->right); //clears right subtree.
        delete currRoot->data;
    }

public:
    AVL<valT, keyT>() : root(nullptr),
                        numOfNodes(0),
                        minNode(nullptr),
                        maxNode(nullptr),
                        updateMinNode(false),
                        updateMaxNode(false){}

    /*
     * constructs AVL tree from sorted Array.
     * time complexity of O(array.size).
     */
    //SortedArray<Pair<Pair<int, int>, Contestant*>>* contestantArrayByStrength1 = this->make_contestant_array_by_strength();
    /*
    explicit AVL<valT, keyT>(SortedArray<Pair<keyT, valT>>* array) : root(nullptr),
                                                                     numOfNodes(0),
                                                                     minNode(nullptr),
                                                                     secondMinNode(nullptr),
                                                                     thirdMinNode(nullptr),
                                                                     maxNode(nullptr),
                                                                     secondMaxNode(nullptr),
                                                                     thirdMaxNode(nullptr),
                                                                     updateMinNode(false),
                                                                     updateSecondMinNode(false),
                                                                     updateThirdMinNode(false),
                                                                     updateMaxNode(false),
                                                                     updateSecondMaxNode(false),
                                                                     updateThirdMaxNode(false){
        this->numOfNodes = array->get_size();
        this->root = sorted_array_to_tree(array->get_sorted_array(), 0,
                                          this->numOfNodes-1); //builds tree from sorted array.

        int indexMin1 = 0, indexMin2 = 0, indexMin3 = 0;
        int indexMax1 = 0, indexMax2 = 0, indexMax3 = 0;
        if (array->get_size() == 2)
        {
            indexMin2 = 1;
            indexMin3 = 1;
            indexMax1 = 1;
        }
        else if (array->get_size() >= 3)
        {
            indexMin1 = 0;
            indexMin2 = 1;
            indexMin3 = 2;
            indexMax1 = array->get_size() - 1;
            indexMax2 = array->get_size() - 2;
            indexMax3 = array->get_size() - 3;
        }

        //get min nodes from built tree.

        // SortedArray<Pair<int, Contestant*>>* newContestantArrayByID1 = new SortedArray<Pair<int, Contestant*>>(arr1Size);
        // SortedArray<Pair<Pair<int, int>, Contestant*>>* contestantArrayByStrength1 = this->make_contestant_array_by_strength();
        Pair<keyT, valT> curr1 = array->get_sorted_array()[indexMin1];
        Pair<keyT, valT> curr2 = array->get_sorted_array()[indexMin2];
        Pair<keyT, valT> curr3 = array->get_sorted_array()[indexMin3];
        this->minNode = this->search_aux(this->root, curr1.get_first());
        this->secondMinNode = this->search_aux(this->root, curr2.get_first());
        this->thirdMinNode = this->search_aux(this->root, curr3.get_first());
        this->updateMinNode = false;
        this->updateSecondMinNode = false;
        this->updateThirdMinNode = false;

        //get max nodes from built tree.
        Pair<keyT, valT> curr11 = array->get_sorted_array()[indexMax1];
        Pair<keyT, valT> curr21 = array->get_sorted_array()[indexMax2];
        Pair<keyT, valT> curr31 = array->get_sorted_array()[indexMax3];
        this->maxNode = this->search_aux(this->root, curr11.get_first());
        this->secondMaxNode = this->search_aux(this->root, curr21.get_first());
        this->thirdMaxNode = this->search_aux(this->root, curr31.get_first());
        this->updateMaxNode = false;
        this->updateSecondMaxNode = false;
        this->updateThirdMaxNode = false;
    }
    */

    ~AVL()
    {
        delete this->root;
    }

    /*
     * destroys tree.
     */
    void clear()
    {
        this->clear_aux(this->root);
        this->root = nullptr;
        this->minNode = nullptr;
        this->maxNode = nullptr;
    }

    /*
     * returns max of to integers.
     */
    static int max(int x, int y) {
        if (x > y) {
            return x;
        }
        return y;
    }

    /*
     * returns true if tree is empty, else false.
     */
    bool is_empty() const {
        return (numOfNodes == 0);
    }

    /*
     * returns number of nodes in tree.
     */
    int get_size() const {
        return this->numOfNodes;
    }

    /*
     * finds and returns data of node in tree with key=key if exists, returns nullptr if not.
     * time complexity of O(log(tree.nunOfNodes)).
     */
    valT search(keyT key) const {
        Node *nodeToFind = search_aux(this->root, key);
        return nodeToFind != nullptr ? nodeToFind->data
                                     : nullptr; //if nodeToFind is not nullptr returns nodeToFind->data, else returns nullptr.
    }

    /*
     * creates new node with parameters T,S and adds it to the tree.
     * return: SUCCESS- node inserted successfully.
     *         ALLOCATION_ERROR- error in memory allocation for new node creation.
     *         FAILURE- the node is already in the tree.
     * time complexity of O(log(tree.nunOfNodes)).
     */
    StatusType insert(valT data, keyT key) {
        try {
            Node *newNode = new Node(data, key); //create new node.
            this->root = insert_aux(this->root, newNode); //insert node to tree.
            if (this->numOfNodes == 0) //if first node in tree.
            {
                this->minNode = newNode;
                this->maxNode = newNode;
            }
            else //general case.
            {
                if (*newNode < *this->minNode) //if new node is smallest node in the tree.
                {
                    this->minNode = newNode;
                }
                if (*newNode > *this->maxNode) //if new node is biggest node in the tree.
                {
                    this->maxNode = newNode;
                }
            }
            this->numOfNodes++;
        }
        catch (std::bad_alloc& err) //allocation of new node failed.
        {
            return StatusType::ALLOCATION_ERROR;
        }
        catch (alreadyInTree& err) //node already in tree.
        {
            return StatusType::FAILURE;
        }

        return StatusType::SUCCESS;
    }

    /*
     * if exists, deletes node with key=key from the tree.
     * return: SUCCESS- node deleted successfully.
     *         ALLOCATION_ERROR- error in freeing of deleted node data.
     *         FAILURE- the node is not in the tree.
     * time complexity of O(log(tree.nunOfNodes)).
     */
    StatusType remove(keyT key) {
        try {
            this->root = remove_aux(this->root, key);
            this->numOfNodes--;

            if (this->numOfNodes == 0)
            {
                this->minNode = nullptr;
                this->maxNode   = nullptr;
            }
            else if (this->numOfNodes == 1)
            {
                this->minNode = this->root;
                this->maxNode   = this->root;
            }
            else // general case.
            {
                if (this->updateMinNode)
                {
                    this->minNode = this->get_min_node();
                }
                else if (this->updateMaxNode)
                {
                    this->maxNode = this->get_max_node();
                }
            }
        }
        catch (std::bad_alloc& err) {
            return StatusType::ALLOCATION_ERROR;
        }
        catch (notInTree& err) {
            return StatusType::FAILURE;
        }
        this->updateMinNode = false;
        this->updateMaxNode = false;
        this->updateSecondMaxNode = false;
        this->updateThirdMaxNode = false;

        return StatusType::SUCCESS;
    }

    /*
     * converts tree to Array and returns the Array.
     * time complexity of O(tree.numOfNodes).
     */
    /*
    SortedArray<Pair<keyT, valT>>* convert_to_sorted_array() const {
        SortedArray<Pair<keyT, valT>>* array = new SortedArray<Pair<keyT, valT>>(this->numOfNodes);
        tree_to_sorted_array(array->get_sorted_array(), 0, this->root);//creates the Array from the tree recursively.
        return array;
    }
     */

    /*
     * returns valT of maxNode.
     * time complexity of O(1).
     */
    valT get_max_node_val() const
    {
        return (this->maxNode != nullptr) ? this->maxNode->data : NULL;
    }

    /*
     * returns valT of minNode.
     * time complexity of O(1).
     */
    valT get_min_node_val() const
    {
        return (this->minNode != nullptr) ? this->minNode->data : NULL;
    }

    /*
     * transfers node with key=keyToTransfer from other tree to this tree.
     * returns status of transfer.
     */
    StatusType transfer_node(AVL<valT, keyT>* other, keyT keyToTransfer) {
        valT dataToTransfer = other->search(keyToTransfer);
        if (dataToTransfer == nullptr) {
            return StatusType::FAILURE;
        }
        other->remove(keyToTransfer);
        return this->insert(dataToTransfer, keyToTransfer);
    }

};





























































































































































































































































































































































































































































































template<class valT, class keyT>
StatusType AVL<valT,keyT>::findRank(int rank);


statusType
template<class valT, class keyT>
StatusType AVL<valT,keyT>::findRank(int rank)
{
    if (rank < 0 || rank > this->numOfNodes)
    {
        return StatusType::FAILURE;
    }
    return this->findRankAux(rank, this->root);
}

StatusType findMedian(){
    // do it ususing the AVL rank, and the AVL size
    // if the size is odd, return the median

}











#endif //WET24_TO_PUBLISH_AVL_H
