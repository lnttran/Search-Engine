#ifndef AVLtree_h 
#define AVLtree_h 
#include <queue>
#include <stack>
#include <iostream>
#include <algorithm>
#define DEBUG 

using namespace std; 

template <typename keyType, typename valueType>
class AVLtree
{
    private: 
        struct TreeNode {
            keyType key; // key type of the map 
            valueType value; //value that associated with its key
            TreeNode* left; 
            TreeNode* right; 
            int height; 

            TreeNode (const keyType &k, const valueType &val , TreeNode* lt, TreeNode* rt, int h = 0)
                : key{k}, value{val}, left{lt}, right {rt}, height{h} {}
        }; 

        TreeNode * root; 

    public: 
        /**
         * @brief Default constructor
         */
        AVLtree() : root(nullptr) {}; 

        /**
         * @brief Destructor
         * Call the internal function to free the tree 
         */
        ~AVLtree() {
            clear(); 
        }

        /**
         * @brief Copy constructor uses internal function clone()
         */
        AVLtree(const AVLtree &rhs) : root{nullptr}
        {
            root = clone(rhs.root); 
        }

        /**
         * @brief Copy assignment constructor 
         */
        AVLtree &operator=(const AVLtree &rhs) {
            clear(); // deallocate memory before assigning 
            root = clone(rhs.root); //copy rhs's elements to lhs
            return *this; 
        }

        /**
         * @brief Insert new node to the tree by calling internal function.
         * 
         * @param k is the key to be inserted.
         */
        void insert(const keyType &k, const valueType &v) {
            insert(root, k , v); 
        }; 

        /**
         * @brief Remove a key from the tree by calling internal function 
         * 
         * @param k is the key to remove.
         */
        void remove (const keyType &k) {
            remove (root, k); 
        }

         /**
         * @brief clear the tree by calling internal function.
         */
        void clear () {
            clear(root); 
        };

        /**
         * @brief Function prints tree using inorder traversal.
         * Calling internal function 
         * @param out 
         */
        void printInorder(ostream &out = cout) const {
            printInorder(root, out); 
        };

        /**
         * @brief Function prints tree using level-order traversal
         * Calling internal function 
         * @param out 
         */
        void printBreadthFirst(ostream &out = cout) const {
            printBreadthFirst(root, out);
        }

        /**
         * @brief Function prints tree structure
         * Calling internal function 
         */
        void prettyPrintTree() const {
            prettyPrintTree("", root, false); 
        }

        /**
         * @brief Find an specific element in the tree
         * calling internal function to find the value in the tree.
         * @return true of x is found
         */
        bool find(const keyType &k) const {
            return find(root, k) != nullptr; 
        }

        /**
         * @brief Check if the tree is empty 
         * 
         * @return true if empty
         * @return false otherwise
         */
        bool isEmpty() const {
            return root == nullptr; 
        }; 

    private: 
        
        /**
         * @brief return the height of node r or -1 if nullptr; 
         */
        int height(TreeNode* r) const {
            return r == nullptr ? -1 : r->height; 
        }; 

        /**
         * @brief Internal function that copy the element of the tree
         * if the tree is empty, return 
         * else create new node initialize with the current node's attributes.
         * @param r is the root node of the tree
         * @return TreeNode* 
         * Question: Do i need to clone the value of the key too (map)?
         */
        TreeNode* clone (TreeNode* r) const {
            if (r == nullptr) return nullptr;
            return new TreeNode{r->key, r->value, clone(r->left), clone(r->right), r->height};
        };

        /**
         * @brief Internal method to insert into a subtree 
         * if there is no node in the subtree, create new nodes with value k, 0 height and 2 null pointer
         * if the value is smaller than the root->key, using recursion to insert a new node to the left side of the root node. 
         * If the value is larger than the root->key, use recursion to insert a new node to the right side of the root node.
         * Otherwise, return nothing due to duplication. 
         * calling balance() function to rebalance the tree. 
         * @param r is a root of subtree
         * @param k the value to insert
         */
        void insert ( TreeNode* &r, const keyType &k, const valueType &v) {
            if ( r == nullptr) 
            {
                r = new TreeNode{k, v, nullptr, nullptr, 0}; 
                return; 
            }

            if (k < r->key)
                insert(r->left, k, v); 
            else if (r->key < k)
                insert(r->right, k, v);
            else 
                return; // if dublicate, return. 
            
            balance(r); 
        }

        /**
         * @brief Internal function to remove the value x from the tree
         * Set the new root to the subtree
         * @param r is the node that roots the tree
         * @param x is the item to remove
         * Question: Is it balanced?
         */
        //coud use TreeNode **r as a pointer to pointer r 
        void remove (TreeNode* &r, const keyType &k) {
            if (r == nullptr)
                return; 
            
            //tranverse to the tree to find the k 
            if (k < r->key)
                return remove(r->left, k); 
            if (r->key < k)
                return remove(r->right, k);

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
                TreeNode *replacement = removeMin(r->right);
                //Assign the right child of the replacement pointer to the right child of the root node
                replacement->right = r->right;
                //Assign the left child of the replacement pointer to the left child of the root node
                replacement->left = r->left;

                //delete the root
                delete r; 
                //replace the root node to the replacement (the smallest item in its right subtree, which contains the root's right and left children)
                r = replacement; 
                return; 
            }

            //Case b: If the node has only child. Replace the node with the only child. 
            TreeNode *oldNode = r; 
            //If the left node is not nullptr, replace the node with left child. Otherwise, right child. 
            r = (r->left != nullptr) ? r->left : r->right;
            delete oldNode; 
            balance(r); 
        };

        /**
         * @brief Internal function to find, unlink from the tree and return the smallest item in the subtree r
         * 
         * @param r is the root of the subtree
         * @return TreeNode* 
         */
        //Question: Explain how this work?
        TreeNode *removeMin(TreeNode *&r) {
            if ( r == nullptr )
                return nullptr; 
            
            //root is the minimum 
            if (r->left == nullptr)
            {
                TreeNode *min = r; 
                r = r->right; 
                return min; 
            }
            //traverse down left to the leaf (node with no children)
            if (r->left->left == nullptr){
                TreeNode *min = r->left; 
                r->left = min->right; 
                return min; 
            }

            return removeMin(r->left); 
        };

        /**
         * @brief Internal function to empty the entire tree
         * using recursion to trace down the the last node of the tree then delete
         * @param r is the root node of the tree
         */
        void clear (TreeNode *&r){
            if (r == nullptr)
                return; 
            clear(r->left); 
            clear(r->right); 
            delete r; 
            r = nullptr;
        }; 

        /**
         * @brief Internal function to find the key value in a tree.
         * 
         * @param r is the root of the tree
         * @param k is the value to search 
         * @return TreeNode* value if found
         */
        TreeNode* find(TreeNode *r , const keyType &k) const {
            if ( r == nullptr)
                return nullptr; 
            if ( k < r->key)
                return find (r->left, k); 
            if ( r->key < k)
                return find (r->right, k);
            //we have the r->key = k; 
            return r;   
        };

        /**
         * @brief this function print out the node using level-order traversal (breath-first)
         * Question: how does the code look in queue. 
         * @param r is the root node
         * @param out for cout the node element
         */
        void printBreadthFirst(TreeNode *r, ostream &out) const {
            queue<TreeNode *> q; //creating a queue of TreeNode pointer  
            TreeNode *current; 
            if (r == nullptr)
                return; //return if the root is empty
            
            //start with the root node in the queue
            q.push(r); 
            
            //Since the root can be a nullptr, therefore we need to check if the tree is empty or not.
            while (!q.empty()){
                //if the tree is not empty, assigning the current node to the current root of the tree
                current = q.front(); //how the current node look like
                q.pop(); //delete the last element of the queue
                out << current->key << " -> " ; //print out the element of the root

                if (current->left != nullptr)
                    q.push(current->left); //add the left child node to the queue
                if (current->right != nullptr)
                    q.push(current->right); //add the right child node to the queue
            }
        };

        /**
         * @brief Internal recursive method to print a subtree rooted at r in sorted order
         * This is inorder traversal (LNR)
         * @param r 
         * @param out 
         */
        void printInorder(TreeNode *r, ostream &o) const {
            if (r == nullptr)
                return;

            // recursion
            printInorder(r->left, o);
            o << r->key << " ";
            printInorder(r->right, o);
        };

        /**
         * @brief Pretty print the tree structure 
         * Uses preorder traversal with R and L swapped (NRL)
         * 
         * @param prefix 
         * @param node 
         * @param isRight 
         */
        void prettyPrintTree(const std::string &prefix, const TreeNode *node, bool isRight) const
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

        static const int ALLOWED_IMBALANCE = 1; 

        /**
         * @brief Function return the the greater value.
         */
        int max(int lhs, int rhs) const {
            return lhs > rhs ? lhs : rhs;
        }

        /**
         * @brief This function reblance the tree.
         * If the height of the left subtree is greater than the height of the right subtree,
         * 
         * @param r 
         */
        void balance (TreeNode* &r){
            if ( r == nullptr)
                return; 
            if (height(r->left) - height(r->right) > ALLOWED_IMBALANCE){
                if (height(r->left->left) >= height(r->left->right))
                    leftRotate(r); 
                else 
                    rightLeftRotate(r); 
            }
            else if (height(r->right) - height(r->left) > ALLOWED_IMBALANCE) {
                if (height(r->right->right) >= height(r->right->left))
                    rightRotate(r); 
                else
                    leftRightRotate(r); 
            }

            r->height = max(height(r->left), height(r->right)) + 1; 
        }

        /**
         * @brief Function rotates binary tree with left child 
         * Update heights, then set new root
         * @param r2 is the root of the subtree
         */
        void leftRotate(TreeNode* &r2){
            #ifdef DEBUG 
                cout << "Need to leftRotate for node " << r2->key << endl;
                cout << "Before: " << endl; 
                prettyPrintTree();
                printBreadthFirst(); 
                cout << endl; 
            #endif 
                TreeNode *r1 = r2->left; 
                r2->left = r1->right; 
                r1->right = r2; 
                r2->height = max(height(r2->left), height(r2->right)) + 1;
                r1->height = max(height(r1->left), r2->height) + 1; 
                r2 = r1; 
            #ifdef DEBUG 
                cout << "After: " << endl; 
                prettyPrintTree();
                printBreadthFirst(); 
                cout << endl;
            #endif

        };

        /**
         * @brief Function rotates binary tree with right child 
         * Update heights, then set new root
         * @param r 
         */
        void rightRotate(TreeNode *&r1){
            #ifdef DEBUG 
                cout << "Need to rightRotate for node " << r1->key << endl; 
                cout << "Before: " << endl; 
                prettyPrintTree(); 
                printBreadthFirst(); 
                cout << endl;
            #endif 
                TreeNode *r2 = r1->right;
                r1->right = r2->left; 
                r2->left = r1; 
                r1->height = max(height(r1->left), height(r1->right))+ 1; 
                r2->height = max(height(r2->right), r1->height) + 1; 
                r1 = r2; 
            #ifdef DEBUG 
                cout << "After: " << endl; 
                prettyPrintTree();
                printBreadthFirst();
                cout << endl; 
            #endif

        }; 
        
        /**
         * @brief Double rotate binary tree node: first right child with its left child; 
         * then node r1 with new right child.
         * Update heights, then set new root.
         * @param r 
         */
        void leftRightRotate(TreeNode *&r1){
            #ifdef DEBUG 
                cout << "Performing leftRightRotate: " << endl;
            #endif
                rightRotate(r1->right); 
                leftRotate(r1); 
        };

        /**
         * @brief Double rotate binary tree node: first left child with its right child
         * then node r3 with new left child. 
         * Update heights, then set new root
         * @param r3 
         */
        void rightLeftRotate(TreeNode* &r3){
            #ifdef DEBUG 
                cout << "Performing rightLeftRotate: " << endl; 
            #endif  
                rightRotate(r3->left);
                leftRotate(r3); 
        };

        



};

#endif