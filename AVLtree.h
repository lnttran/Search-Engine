#ifndef AVL_TREE_H
#define AVL_TREE_H

#define DEBUG

#include <stdexcept>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <stack>
#include <queue> // for level-order traversal

using namespace std;

// AvlTree class

template <typename keyType, typename valueType>
class AvlTree
{
private:
    struct AvlNode
    {
        keyType key; //key type of the map
        valueType value; //value that associated with its key
        AvlNode *left;
        AvlNode *right;
        int height;
        int size;

        //Initialize
        AvlNode(const keyType &ele, const valueType &val, AvlNode *lt, AvlNode *rt, int h = 0)
                : key{ele}, value{val}, left{lt}, right{rt}, height{h} {}
    };
    AvlNode *root;

public:
    /**
     * @brief Default constructor
     */
    AvlTree() : root{nullptr}{}

    /**
     * @brief Rule-of-3 Part 1: Copy constructor uses internal function clone().
     *
     */
    AvlTree(const AvlTree &rhs) : root{nullptr}
    {
        root = clone(rhs.root);
    }

    /**
     * @brief Rule-of-3 Part 2: Destroy the Binary Search Tree object using the internal
     *   function clear().
     *  Detructure
     */
    ~AvlTree()
    {
        clear();
    }

    /**
     * @brief Rule-of-3 Part 1: Copy constructor uses internal function clone().
     */
    AvlTree &operator=(const AvlTree &rhs)
    {
        clear(); //dealocate memory before assigning 
        root = clone(rhs.root); //copy rhs's element to lhs 
        return *this;
    }

    /**
     * Returns true if x is found in the tree.
     */
    bool find(const keyType &x) const
    {
        return find(x, root);
    }

    /**
    * Return unordered map
    */
    valueType* findGetMap(const keyType &x) const
    {
        AvlNode* node = findNode(x, root);
        if (node == nullptr) {
            return nullptr;
        }
        return &(node->value);
    }

    AvlNode* rootNode() const {
        return root;
    }

    AvlNode* findNode(const keyType &k) const {
        return findNode(k, root);
    }

    /**
     * Test if the tree is logically empty.
     * Return true if empty, false otherwise.
     */
    bool isEmpty() const
    {
        return root == nullptr;
    }

    /**
     * Function prints tree using inorder traversal.
     * Calling internal function 
     */
    void printInorder(ostream &out = cout) const
    {
        printInorder(root, out);
    }

    /**
     * @brief Function prints tree using level-order traversal
     * Calling internal function 
     * @param out 
     */
    void printBreadthFirst(ostream &out = cout) const
    {
        printBreadthFirst(root, out);
    }

    /**
     * Print the tree structure.
     */
    void prettyPrintTree() const
    {
        prettyPrintTree("", root, false);
    }

    /**
     * Make the tree empty.
     */
    void clear()
    {
        clear(root);
    }

    /**
     * Insert x into the tree; duplicates are ignored.
     */
    void insert(const keyType &x, const valueType &v)
    {
        insert(x, v,  root);
    }

    /**
     * Remove x from the tree. Nothing is done if x is not found.
     */
    void remove(const keyType &x)
    {
        remove(x, root);
    }

    void displayInfo(ostream &out = cout) {
        displayInfo (root, out);
    }

    void displayInfoToFile (fstream &file) {
        displayInfoToFile_internal(root, file);
    }

    int numOfNode() const {
        return numOfNode(root);
    }


private:

    /**
     * @brief Internal method to insert into a subtree 
     * if there is no node in the subtree, create new nodes with value k, 0 height and 2 null pointer
     * if the value is smaller than the root->key, using recursion to insert a new node to the left side of the root node. 
     * If the value is larger than the root->key, use recursion to insert a new node to the right side of the root node.
     * Otherwise, return nothing due to duplication. 
     * calling balance() function to rebalance the tree. 
     * @param t is a root of subtree
     * @param x the key to insert
     * @param v the value associated with the key
     */
    void insert(const keyType &x, const valueType &v, AvlNode* &t)
    {
        if (t == nullptr)
        {
            t = new AvlNode{x, v, nullptr, nullptr, 0};
            return; // a single node is always balanced
        }

        if (x < t->key)
            insert(x, v, t->left);
        else if (t->key < x)
            insert(x, v, t->right);
        else
        {
        } // Duplicate; do nothing

        balance(t);
    }

    /**
     * @brief Internal function to find the smallest item in the subtree, then unlink that item from the tree
     * return to the individual pointer of the smallest item
     * 
     * @param r is the root of the subtree
     * @return TreeNode* 
     */

    AvlNode *removeMin(AvlNode *&r) {
        if ( r == nullptr )
            return nullptr;

        //root is the minimum 
        if (r->left == nullptr)
        {
            AvlNode *min = r;
            r = r->right;
            return min;
        }
        //traverse down left to the leaf (node with no children)
        if (r->left->left == nullptr){
            AvlNode *min = r->left;
            r->left = min->right;
            return min;
        }

        return removeMin(r->left);
    };


    /**
     * @brief Internal function to remove the value x from the tree
     * Set the new root to the subtree
     * @param r is the node that roots the tree
     * @param k is the item to remove
     * Question: Is it balanced?
     */
    //coud use TreeNode **r as a pointer to pointer r 
    void remove(const keyType &k, AvlNode *&r)
    {
        if (r == nullptr)
            return;

        //tranverse to the tree to find the k 
        if (k < r->key)
            return remove(k, r->left);
        if (r->key < k)
            return remove(k, r->right);

        //if k == r->key
        //perform the following code

        //Case a: If the node has no children, delete node. 
        if (r->left == nullptr && r->right == nullptr) {
            delete r;
            r = nullptr;
            return;
        }

        //Case c: if the node has two children, replace key with the smallest key on the right subtree. 
        //Question: if call the removeMin() function, will this affect the right subtree since it also remove the smallest item?
        //relink node, take the node out of the tree and replace it to other position 
        if (r->left != nullptr && r->right != nullptr){
            //create a pointer points to the smallest item in the right subtree of the root node.  
            AvlNode *replacement = removeMin(r->right);
            //Assign the right child of the replacement pointer to the right child of the root node
            replacement->right = r->right;
            //Assign the left child of the replacement pointer to the left child of the root node
            replacement->left = r->left;

            //delete the root
            delete r;
            //replace the root node to the replacement (the smallest item in its right subtree, which find the root's right and left children)
            r = replacement;
            return;
        }

        //Case b: If the node has only child. Replace the node with the only child. 
        AvlNode *oldNode = r;
        //If the left node is not nullptr, replace the node with left child. Otherwise, right child. 
        r = (r->left != nullptr) ? r->left : r->right;
        delete oldNode;

        // don't forget to balance
        balance(r);
    }

    /**
     * Internal method to find the smallest item in a subtree t.
     * Return node containing the smallest item.
     */
    AvlNode *findMin(AvlNode *t) const
    {
        if (t == nullptr)
            return nullptr;

        if (t->left == nullptr)
            return t;

        return findMin(t->left);
    }

    /**
     * Internal method to test if an item is in a subtree.
     * x is item to search for.
     * t is the node that roots the tree.
     */
    bool find(const keyType &x, AvlNode *t) const
    {
        if (t == nullptr)
            return false;

        else if (x < t->key)
            return find(x, t->left);
        else if (t->key < x)
            return find(x, t->right);
        else
            return true; // Match
    }


    /**
     * @brief Internal function to find the key value in a tree.
     * 
     * @param r is the root of the tree
     * @param k is the value to search 
     * @return TreeNode* value if found
     */
    AvlNode* findNode(const keyType &k, AvlNode *r) const {
        if ( r == nullptr)
            return nullptr;
        if ( k < r->key)
            return findNode ( k, r->left);
        if ( r->key < k)
            return findNode (k, r->right);
        //we have the r->key = k; 
        return r;
    };

    /**
      * @brief Internal function to empty the entire tree
      * using recursion to trace down the the last node of the tree then delete
      * @param r is the root node of the tree
      */
    void clear(AvlNode *&t)
    {
        if (t == nullptr)
            return;

        clear(t->left);
        clear(t->right);
        t = nullptr;
    }

    /**
     * @brief Internal function that copy the element of the tree
     * if the tree is empty, return 
     * else create new node initialize with the current node's attributes.
     * @param r is the root node of the tree
     * @return TreeNode* 
     * Question: Do i need to clone the value of the key too (map)?
     */
    AvlNode* clone(AvlNode *t) const
    {
        if (t == nullptr)
            return nullptr;

        return new AvlNode{t->key, t->value, clone(t->left), clone(t->right), t->height};
    }

    /**
     * @brief Return number of nodes in the tree
     * 
     * @param r root
     * @return int 
     */
    int numOfNode( AvlNode* r) const {

        if (r == nullptr)
            return 0;

        return 1 + numOfNode(r->left) + numOfNode(r->right);
    }

    void displayInfo ( AvlNode* &r, ostream &out) {
        if (r == nullptr){
            return;
        }

        displayInfo(r->left, out);

        out << r->key << ": " << endl;
        for (auto it = r->value.begin(); it != r->value.end(); ++it) {
            out << it->first << ", " << it->second << endl;
        }
        cout << endl;

        displayInfo(r->right, out);

    }

    /**
     * @brief Display information of the tree to file
     * 
     * @param r root node
     * @param file file namee 
     */
    void displayInfoToFile_internal(AvlNode* &r, fstream &file) {
        if( r == nullptr)
            return;

        displayInfoToFile_internal(r->left, file);
        file << r->key << ",";
        for ( auto it = r->value.begin(); it != r->value.end(); ++it )
            file << it->first << " " << it->second << ",";
        file << endl;
        displayInfoToFile_internal(r->right, file);
    }

    /**
     * @brief Internal recursive method to print a subtree rooted at r in sorted order
     * This is inorder traversal (LNR)
     * @param t
     * @param out 
     */
    void printInorder(AvlNode *t, ostream &out) const
    {
        if (t == nullptr)
            return;

        // recursion
        printInorder(t->left, out);
        out << t->key << " ";
        printInorder(t->right, out);
    }

    /**
     * @brief this function print out the node using level-order traversal (breath-first)
     * Question: how does the code look in queue. 
     * @param r is the root node
     * @param out for cout the node element
     */
    void printBreadthFirst(AvlNode *t, ostream &out) const
    {
        if (t == nullptr)
            return; //return if the root is empty

        AvlNode *current;
        queue<AvlNode *> q; //creating a queue of TreeNode pointer 

        // start with the root node in the queue
        q.push(t);

        //Since the root can be a nullptr, therefore we need to check if the tree is empty or not.
        while (!q.empty())
        {
            //if the tree is not empty, assigning the current node to the current root of the tree
            current = q.front();
            q.pop();
            out << current->key << " -> ";

            // add children to the end of the queue
            if (current->left != nullptr)
                q.push(current->left); //add the left child node to the queue

            if (current->right != nullptr)
                q.push(current->right); //add the right child node to the queue
        }
    }

    /**
     * @brief Pretty print the tree structure 
     * Uses preorder traversal with R and L swapped (NRL)
     * 
     * @param prefix 
     * @param node 
     * @param isRight 
     */
    void prettyPrintTree(const std::string &prefix, const AvlNode *node, bool isRight) const
    {
        if (node == nullptr)
            return;

        std::cout << prefix;
        // Note: this uses unicode characters for the tree structure. They might not print correctly on 
        // all systems (Windows!?!) and all types of output devices.
        std::cout << (isRight ? "├──" : "└──");
        // print the value of the node
        std::cout << node->key << std::endl;

        // enter the next tree level - left and right branch
        prettyPrintTree(prefix + (isRight ? "│   " : "    "), node->right, true);
        prettyPrintTree(prefix + (isRight ? "│   " : "    "), node->left, false);
    }

    // Avl manipulations

    /**
     * Return the height of node t or -1 if nullptr.
     */
    int height(AvlNode *t) const
    {
        return t == nullptr ? -1 : t->height;
    }

    static const int ALLOWED_IMBALANCE = 1; // 1 is the default; more will make balancing cheaper
    // but the search less efficient.


    void balance(AvlNode *&t)
    {
        if (t == nullptr)
            return;

        if (height(t->left) - height(t->right) > ALLOWED_IMBALANCE) // unbalancing insertion was left
        {
            if (height(t->left->left) >= height(t->left->right))
                rotateWithLeftChild(t); // case 1 (outside)
            else
                doubleWithLeftChild(t); // case 2 (inside)
        }
        else if (height(t->right) - height(t->left) > ALLOWED_IMBALANCE) // unbalancing insertion was right
        {
            if (height(t->right->right) >= height(t->right->left))
                rotateWithRightChild(t); // case 4 (outside)
            else
                doubleWithRightChild(t); // case 3 (inside)
        }

        // update height
        t->height = max(height(t->left), height(t->right)) + 1;
    }

    int max(int lhs, int rhs) const
    {
        return lhs > rhs ? lhs : rhs;
    }

    /**
     * Rotate binary tree node with left child.
     * For AVL trees, this is a single rotation for case 1.
     * Update heights, then set new root.
     */
    void rotateWithLeftChild(AvlNode *&k2)
    {
// #ifdef DEBUG
//         cout << "need to rotateWithLeftChild for node " << k2->key << endl;
//         cout << "Before:" << endl;
//         prettyPrintTree();
// #endif

        AvlNode *k1 = k2->left;
        k2->left = k1->right;
        k1->right = k2;
        k2->height = max(height(k2->left), height(k2->right)) + 1;
        k1->height = max(height(k1->left), k2->height) + 1;
        k2 = k1;
// #ifdef DEBUG
//         cout << "After:" << endl;
//         prettyPrintTree();
// #endif
    }

    /**
     * Rotate binary tree node with right child.
     * For AVL trees, this is a single rotation for case 4.
     * Update heights, then set new root.
     */
    void rotateWithRightChild(AvlNode *&k1)
    {
// #ifdef DEBUG
//         cout << "need to rotateWithRightChild for node " << k1->key << endl;
//         cout << "Before:" << endl;
//         prettyPrintTree();
// #endif
        AvlNode *k2 = k1->right;
        k1->right = k2->left;
        k2->left = k1;
        k1->height = max(height(k1->left), height(k1->right)) + 1;
        k2->height = max(height(k2->right), k1->height) + 1;
        k1 = k2;
// #ifdef DEBUG
//         cout << "After:" << endl;
//         prettyPrintTree();
// #endif
    }

    /**
     * Double rotate binary tree node: first left child.
     * with its right child; then node k3 with new left child.
     * For AVL trees, this is a double rotation for case 2.
     * Update heights, then set new root.
     */
    void doubleWithLeftChild(AvlNode *&k3)
    {
// #ifdef DEBUG
//         cout << "doubleWithLeftChild" << endl;
// #endif
        rotateWithRightChild(k3->left);
        rotateWithLeftChild(k3);
    }

    /**
     * Double rotate binary tree node: first right child.
     * with its left child; then node k1 with new right child.
     * For AVL trees, this is a double rotation for case 3.
     * Update heights, then set new root.
     */
    void doubleWithRightChild(AvlNode *&k1)
    {
// #ifdef DEBUG
//         cout << "doubleWithRightChild" << endl;
// #endif
        rotateWithLeftChild(k1->right);
        rotateWithRightChild(k1);
    }

};

#endif