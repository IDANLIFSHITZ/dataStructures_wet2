//
// Created by magshimim on 3/27/2024.
//

#include <iostream>
#include <functional>
#include "wet2util.h"
#include "Pair.h"

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
        int maxSubtreeRank;


        explicit Node(valT data, const keyT& key, Node* parent = nullptr) :
            key(key),
            data(data),
            height(0),
            subtreeSize(0),
            parent(parent),
            left(nullptr),
            right(nullptr),
            extra(0),
            maxSubtreeRank(0){}

        ~Node()
        {
            delete this->left;
            delete this->right;
            return;
        }

        /*
         * operators on Node class:
         * assumes keyT has operators: ==, !=, <, >.
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
        * updates height of node;
        */
        static void update_node_height(Node* node)
        {
            if (node != nullptr)
            {
                node->height = max(get_height(node->left), get_height(node->right)) + 1;
            }
        }

        /*
        * returns subtreeSize of node.
        */
        static int get_subtreeSize(Node *node) {
            if (node == nullptr) {
                return 0;
            }
            return node->subtreeSize;
        }

        /*
         * updates subtreeSize of node.
         */
        static void update_subtreeSize(Node* node)
        {
            if (node != nullptr)
            {
                node->subtreeSize = update_subtreeSize(node->left) + update_subtreeSize(node->right) + 1;
            }
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
    };

    Node *root; //root of AVL tree.
    int numOfNodes;

    Node* minNode; //ptr to smallest node in the tree.
    Node* maxNode; //ptr to biggest node in the tree.

    /*
    * updates maxSubtreeRank of node.
    */
    void update_maxSubtreeRank(Node* node)
    {
        int currRank = this->calc_power(node->data) ;
        int maxRankLeft = (node->left != nullptr) ? // get left maxSubtreeRank.
                          node->left->maxSubtreeRank : 0;
        int maxRankRight = (node->right != nullptr) ? // get right maxSubtreeRank.
                           node->right->maxSubtreeRank : 0;

        node->maxScoreInSubTree = AVL::max(currRank, AVL::max(maxRankLeft, maxRankRight)); //calc maxSubtreeRank.
        node->maxScoreInSubTree += (node->maxScoreInSubTree != 0)
                                   ? node->extra : 0;
    }

    /*
     * executes right rotate. returns original left node.
     */
    Node *right_rotate(Node *nodeToRotate)
    {
        /*
         * part 1: rotate.
        */
        Node* leftNode = nodeToRotate->left;
        int nodeToRotateExtra = nodeToRotate->extra; // save extra of nodeToRotate.
        int leftNodeExtra = leftNode->extra; // save extra of nodeToRotate->left.

        //update extra field.
        nodeToRotate->extra = -1 * leftNodeExtra;
        leftNode->extra += nodeToRotateExtra;

        // updates new left child parent and extra.
        nodeToRotate->left = leftNode->right;
        if (leftNode->right != nullptr) // if left child has right child (new left child).
        {
            leftNode->right->extra += leftNodeExtra;
            leftNode->right->parent = nodeToRotate;
        }

        //update leftNode to be new nodeToRotate.
        leftNode->right = nodeToRotate;
        leftNode->parent = nodeToRotate->parent;
        nodeToRotate->parent = leftNode;


        // updates org parent children.
        if (leftNode->parent != nullptr) // if nodeToRotate had parent.
        {
            if (nodeToRotate->key < leftNode->parent->key) // if was left child.
            {
                leftNode->parent->left = leftNode;
            }
            else  // else was right child.
            {
                leftNode->parent->right = leftNode;
            }
        }

        /*
         * part 2: update heights, subtreeSizeRank, max.
        */
        // update nodeToRotate:
        //update height.
        Node::update_node_height(nodeToRotate);

        //update subtreeSize.
        Node::update_subtreeSize(nodeToRotate);

        //update maxSubtreeRank.
        if (nodeToRotate->left != nullptr)
        {
            this->update_maxSubtreeRank(nodeToRotate->left);
        }
        this->update_maxSubtreeRank(nodeToRotate);
        ///////////////////////////////////////////////////////////////////

        // update leftNode:
        //update height.
        Node::update_node_height(leftNode);

        //update subtreeSize.
        Node::update_subtreeSize(leftNode);

        //update maxSubtreeRank.
        this->update_maxSubtreeRank(leftNode);
        ///////////////////////////////////////////////////////////////////

        return leftNode; //return new nodeToRotate.
    }

    /*
     * executes right rotate. returns original right node.
     */
    Node* left_rotate(Node *nodeToRotate) {
        /*
         * part 1: rotate.
        */
        Node* rightNode = nodeToRotate->right;
        int nodeToRotateExtra = nodeToRotate->extra; // save extra of nodeToRotate.
        int rightNodeExtra = rightNode->extra; // save extra of nodeToRotate->left.

        //update extra field.
        nodeToRotate->extra = -1 * rightNodeExtra;
        rightNode->extra += nodeToRotateExtra;

        // updates new right child parent and extra.
        nodeToRotate->right = rightNode->left;
        if (rightNode->left != nullptr) // if left child has right child (new left child).
        {
            rightNode->left->extra += rightNodeExtra;
            rightNode->left->parent = nodeToRotate;
        }

        //update rightNode to be new nodeToRotate.
        rightNode->left = nodeToRotate;
        rightNode->parent = nodeToRotate->parent;
        nodeToRotate->parent = rightNode;


        // updates org parent children.
        if (rightNode->parent != nullptr) // if nodeToRotate had parent.
        {
            if (nodeToRotate->key < rightNode->parent->key) // if was left child.
            {
                rightNode->parent->left = rightNode;
            }
            else  // else was right child.
            {
                rightNode->parent->right = rightNode;
            }
        }

        /*
         * part 2: update heights, subtreeSizeRank, max.
        */
        // update nodeToRotate:
        //update height.
        Node::update_node_height(nodeToRotate);

        //update subtreeSize.
        Node::update_subtreeSize(nodeToRotate);

        //update maxSubtreeRank.
        if (nodeToRotate->right != nullptr)
        {
            this->update_maxSubtreeRank(nodeToRotate->right);
        }
        this->update_maxSubtreeRank(nodeToRotate);
        ///////////////////////////////////////////////////////////////////

        // update rightNode:
        //update height.
        Node::update_node_height(rightNode);

        //update subtreeSize.
        Node::update_subtreeSize(rightNode);

        //update maxSubtreeRank.
        this->update_maxSubtreeRank(rightNode);
        ///////////////////////////////////////////////////////////////////

        return rightNode; //return new nodeToRotate.
    }

    /*
     * checks if NodeToBalance needs to be balanced and balances it.
     */
    Node* balance(Node* nodeToBalance)
    {
        int balanceFactor = nodeToBalance->get_balance();
        //LL rotate.
        if (balanceFactor > 1 && *nodeToBalance < *nodeToBalance->left)
        {
            nodeToBalance = right_rotate(nodeToBalance);
        }
            //RR rotate.
        else if (balanceFactor < -1 && *nodeToBalance > *nodeToBalance->right)
        {
            nodeToBalance = left_rotate(nodeToBalance);
        }
            //LR rotate.
        else if (balanceFactor > 1 && *nodeToBalance > *nodeToBalance->left)
        {
            nodeToBalance->left = left_rotate(nodeToBalance->left);
            nodeToBalance = right_rotate(nodeToBalance);
        }
            //RL rotate.
        else if (balanceFactor < -1 && *nodeToBalance < *nodeToBalance->right)
        {
            nodeToBalance->right = right_rotate(nodeToBalance->right);
            nodeToBalance = left_rotate(nodeToBalance);
        }

        return nodeToBalance;
    }

    /*
    * returns Node with key=key if exists, else return nullptr.
    */
    Node *search_aux(Node *currNode, const keyT& key) const {
        if (currNode == nullptr) //if in empty leaf.
        {
            return nullptr;
        }
        else if (currNode->key > key) //if key is in left subtree.
        {
            return search_aux(currNode->left, key);
        }
        else if (currNode->key < key) //if key is in right subtree.
        {
            return search_aux(currNode->right, key);
        }
        return currNode; //currNode is the node to find.
    }

    /*
     * adds a new node to the tree recursively.
     * returns the node that in supposed to be the current node in the recursion after insertion and rotations.
     */
    Node *insert_aux(Node *currNode, Node *newNode)
    {
        /* part 1: normal tree insertion. */

        if (currNode == nullptr) //if arrived at leaf.
        {
            this->update_maxSubtreeRank(newNode);
            return newNode;
        }

        //subtract current node extra.
        newNode->extra -= currNode->extra;

        if (*newNode < *currNode) //if newNode belong in left subtree.
        {
            currNode->left = insert_aux(currNode->left, newNode);
            if (currNode->left != nullptr) //update new left child parent.
            {
                currNode->left->parent = currNode;
            }
        }
        else if (*newNode > *currNode) //if newNode belong in right subtree.
        {
            currNode->right = insert_aux(currNode->right, newNode);
            if (currNode->right != nullptr) //update new right child parent.
            {
                currNode->right->parent = currNode;
            }
        }
        else // newNode already in AVL tree.
        {
            throw alreadyInTree();
        }

        /* part 2: update height, subtreeSize, maxSubtreeRank: */

        Node::update_node_height(currNode);
        Node::update_subtreeSize(currNode);
        this->update_maxSubtreeRank(currNode);

        /* part 3: calc balance factor and perform rotations if tree  is unbalanced */

        return this->balance(currNode);
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
                temp->parent = currNode->parent;
                currNode->parent = nullptr;
                currNode->left = nullptr;
                currNode->right = nullptr;
                delete currNode; // deletes old node.

                currNode = temp; //currNode = son if exists, if not nullptr.
            }
            else //currNode has two children.
            {
                Node* nextBiggest = get_min_node(currNode->right); //nextBiggest is the smallest node that is bigger then currNode.

                Node* node2Delete = currNode;
                Node* node2DeleteLeft = currNode->left;
                Node* node2DeleteRight = currNode->right;

                // update children.
                node2Delete->right = nextBiggest->right;
                node2Delete->left = nextBiggest->left; //nextBiggest->left = nullptr (always).

                //update currNode to be next biggest node in subtree.
                currNode = nextBiggest;
                currNode->left = node2DeleteLeft;
                currNode->right = node2DeleteRight;

                if (nextBiggest == nextBiggest->parent) //if next biggest is currNode->right.
                {
                    currNode->right = node2Delete;
                }
                else //if next biggest is not currNode->right (somewhere in subtree.).
                {
                    nextBiggest->parent->left = node2Delete;
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

        return this->balance(currNode);
    }

    /*
     * adds current node to Array at index and adds subtrees.
     * returns updated index after all insertions.
     */
    int tree_to_sorted_array(valT* values, keyT* keys, int index, Node* currNode) const {
        if (currNode == nullptr)
        {
            return index;
        }
        index = tree_to_sorted_array(values, keys, index, currNode->left); //adds left subtree to Array.
        values[index] = currNode->data;
        values[index++] = currNode->key;
        index = tree_to_sorted_array(values, keys, index, currNode->right); //adds right subtree to Array.
        return index;
    }


    /*
     * builds tree from sorted array, returns root of recursive tree.
     */
    Node* sorted_array_to_tree(valT* values, keyT* keys, int start, int end)
    {
        if (start > end)
        {
            return nullptr;
        }
        Node* newRoot = nullptr;
        int mid = (start + end) / 2;
        try
        {
            newRoot = new Node(values[mid], keys[mid]); //create new root of recursive tree.
            int leftSubtreeSize = 0, rightSubtreeSize = 0;

            //create left child subtree from array.
            newRoot->left = sorted_array_to_tree(values, keys, start, mid - 1); //create left subtree recursively.
            if (newRoot->left != nullptr) //update new left child parent.
            {
                newRoot->left->parent = newRoot;
                leftSubtreeSize = newRoot->left->subtreeSize;
            }

            //create right child subtree from array.
            newRoot->right = sorted_array_to_tree(values, keys, mid + 1, end); //create right subtree recursively.
            if (newRoot->right != nullptr) //update new right child parent.
            {
                newRoot->right->parent = newRoot;
                rightSubtreeSize = newRoot->right->subtreeSize;
            }

            //update newRoot.
            this->update_node_height(newRoot);
            newRoot->subtreeSize = leftSubtreeSize + rightSubtreeSize + 1;

            return newRoot;
        }
        catch (std::bad_alloc& err)
        {
            if (newRoot != nullptr)
            {
                if (newRoot->parent != nullptr) //if already updated parent.
                {
                    if (newRoot->parent->left == newRoot) //if left child.
                    {
                        newRoot->parent->left = nullptr;
                    }
                    else //is right child.
                    {
                        newRoot->parent->right = nullptr;
                    }
                }
                delete newRoot;
            }
            throw std::bad_alloc();
        }
        return nullptr;
    }

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

    /*
     * creates new tree and copies all values and keys from tree.
     */
    Node* hard_copy(Node* currNode, Node* newParent)
    {
        if (currNode == nullptr)
        {
            return nullptr;
        }
        Node* newCopy = nullptr;
        try
        {
            int leftSubtreeSize = 0, rightSubtreeSize = 0;
            newCopy = new Node(currNode->data, currNode->key, newParent);

            //hard copy left subtree.
            newCopy->left = hard_copy(currNode->left, newCopy);
            if (newCopy->left != nullptr)
            {
                leftSubtreeSize = newCopy->left->subtreeSize;
            }

            //hard copy right subtree.
            newCopy->right = hard_copy(currNode->right, newCopy);
            if (newCopy->right != nullptr)
            {
                rightSubtreeSize = newCopy->right->subtreeSize;
            }

            //update newCopy.
            this->update_node_height(newCopy);
            newCopy->subtreeSize = leftSubtreeSize + rightSubtreeSize + 1;

            return newCopy;
        }
        catch (std::bad_alloc& err)
        {
            if (newCopy != nullptr)
            {
                if (newCopy->parent != nullptr) //if already updated parent.
                {
                    if (newCopy->parent->left == newCopy) //if left child.
                    {
                        newCopy->parent->left = nullptr;
                    }
                    else //is right child.
                    {
                        newCopy->parent->right = nullptr;
                    }
                }
                delete newCopy;
            }
            throw std::bad_alloc();
        }
    }

public:
    /*
     * public members:
     */
    std::function<int(const valT&)> calc_power;

    ////////////////////////////////

    /*
     * AVL constructor for non team AVL trees that don't need calc_power function.
     */
    AVL<valT, keyT>() : root(nullptr),
                        numOfNodes(0),
                        minNode(nullptr),
                        maxNode(nullptr),
                        calc_power([](const valT&)->int{return 0;}){}

    /*
     * AVL constructor for team AVL trees.
     */
    AVL<valT, keyT>(std::function<int(const valT&)> calc_power) : root(nullptr),
                                                                  numOfNodes(0),
                                                                  minNode(nullptr),
                                                                  maxNode(nullptr),
                                                                  calc_power(calc_power){}

    /*
     * AVL constructor from sorted Array.
     * time complexity of O(array.size).
     */

    AVL<valT, keyT>(valT* values, keyT* keys, int size) : numOfNodes(size),
                                                          calc_power([](const valT&)->int{return 0;})
    {
        this->root = sorted_array_to_tree(values, keys, 0, this->numOfNodes-1); //builds tree from sorted array.

        //get min and max nodes from built tree.
        this->minNode = this->get_min_node();
        this->maxNode = this->get_max_node();
    }

    /*
     * destructor, destructs the tree recursively from root.
     */
    ~AVL()
    {
        delete this->root;
    }

    /*
     * destroys tree content.
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
    StatusType remove(keyT key)
    {
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
                this->minNode = this->get_min_node();
                this->maxNode = this->get_max_node();
            }
        }
        catch (std::bad_alloc& err)
        {
            return StatusType::ALLOCATION_ERROR;
        }
        catch (notInTree& err)
        {
            return StatusType::FAILURE;
        }

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

StatusType treeToArray();




#endif //WET24_TO_PUBLISH_AVL_H
