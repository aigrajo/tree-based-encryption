/*
* Project 3 -- Tree-Based Encryption
* Filename: bst-student-proj3.h
* Student name: Aiden Grajo
*
* Last Changed: 2/29/2024
* This file contains the implementation of the BSTNode, BST,
* and EncryptionTree class methods.
*
*/

#ifndef BST_STUDENT_PROJ3
#define BST_STUDENT_PROJ3

#include "bst-prof-proj3.h"

/* Place your implementation of the BSTNode, BST, and EncryptionTree classes
 * here.
 *
 * Your driver should #include this file, which (as you can see above) will also
 * #include the professor file.
 */


/*BSTNode Destructor
 * Parameters:
 *     none
 *
 * return value: none
 *
 * deletes the left and right children of a node
 */
template <class Base>
BSTNode<Base>::~BSTNode() {
    delete left;
    delete right;
}

/*printPreorder
 * Parameters:
 *     os -- output stream we want to output to
 *     indent -- amount of space we want between each output
 *
 * return value: none
 *
 * prints the tree in preorder recursively
 */
template <class Base>
void BSTNode<Base>::printPreorder(ostream &os, string indent) const {
    os << indent << data << endl;
    if (left) {
        left->printPreorder(os, indent + "  ");
    }
    else {
        os  << indent + "  NULL" << endl;
    }
    if (right) {
        right->printPreorder(os, indent + "  ");
    }
    else {
        os  << indent + "  NULL" << endl;
    }

}

/*minNode
 * Parameters:
 *     none
 *
 * return value: the left most node in the tree
 *
 * called to get the left most node in the tree
 */
template <class Base>
const BSTNode<Base>* BSTNode<Base>::minNode() const {
    const BSTNode<Base> *current = this;
    while (current->left) {
        current = current->left;
    }
    return current;
}

/*maxNode
 * Parameters:
 *     none
 *
 * return value: the right most node in the tree
 *
 * called to get the right most node in the tree
 */
template <class Base>
const BSTNode<Base>* BSTNode<Base>::maxNode() const {
    const BSTNode<Base> *current = this;
    while (current->right) {
        current = current->right;
    }
    return current;
}

/*encrypt
 * Parameters:
 *     item -- the plain item we want to encrypt
 *
 * return value: encrypted path
 *
 * encrypts the path to the item in the codebook
 */
template <class Base>
string EncryptionTree<Base>::encrypt(const Base &item) const {
    const BSTNode<Base> *curr = this->root;

    string path = "r";
    bool exists = false;

    while(curr) {
        if(item < curr->getData()) {
            path += "0";
            curr = curr->getLeft();
        }
        else if (curr->getData() < item){
            path += "1";
            curr = curr->getRight();
        }
        else {
            curr = nullptr;
            exists = true;
        }
    }

    if (!exists) {
        path = "?";
    }
    return path;
}

/*insert
 * Parameters:
 *     item -- the item we want to insert into the codebook
 *             tree
 *
 *
 * return value: none
 *
 * inserts the item into the tree, unless the item already
 * exists in the tree
 */
template <class Base>
void BST<Base>::insert(const Base &item)  {
    BSTNode<Base> *tmp = new BSTNode<Base>(item);
    BSTNode<Base> *prev = nullptr;
    BSTNode<Base> *current = this->root;

    if (!this->root) {
        this->root = tmp;
        return;
    }

    while (current) {
        prev = current;
        if (item < current->data) {
            current = current->left;
        }
        else if (current->data < item) {
            current = current->right;
        }
        else {
            delete tmp;
            return;
        }
    }

    if (item < prev->data) {
        prev->left = tmp;
    }
    else {
        prev->right = tmp;
    }
}

/*decrypt
 * Parameters:
 *     item -- encrypted text we want to decrypt
 *
 * return value: the path to the decrypted string,
 *               or nullptr if it doesn't exist
 *
 * decrypts an encrypted string, returns null if the string is not
 * in the codebook
 */
template <class Base>
const Base* EncryptionTree<Base>::decrypt(const string &path) const {
    const BSTNode<Base> *curr = this->root;

    size_t i = 0;
    while (i < path.size() && curr) {
        if (path[i] == '0') {
            curr = curr->getLeft();
        }
        else if(path[i] == '1') {
            curr = curr->getRight();
        }
        i++;
    }

    if(!curr) {
        return nullptr;
    }

    return &curr->getData();
}

/*remove
 * Parameters:
 *     item -- item we want to remove from the tree
 *
 * return value: none
 *
 * removes the given item from the tree if it exists
 */
template <class Base>
void BST<Base>::remove(const Base &item) {
    BSTNode<Base> *parent = nullptr;
    BSTNode<Base> *remove = this->root;
    bool exists = false;

    while(remove && !exists) {
        if (item < remove->data) {
            parent = remove;
            remove = remove->left;
        }
        else if (remove->data < item ) {
            parent = remove;
            remove = remove->right;
        }
        else {
            exists = true;
        }
    }

    if (!exists) {
        return;
    }

    //2 children
    if(remove->getLeft() && remove->getRight()) {
        BSTNode<Base> *leftMost = remove->right;
        BSTNode<Base> *leftMostParent = remove;

        if (leftMost->getLeft()) {
            while(leftMost->getLeft()) {
                leftMostParent = leftMost;
                leftMost = leftMost->left;
            }
            leftMostParent->left = leftMost->right;
            leftMost->right = remove->right;
        }

        leftMost->left = remove->left;

        if (parent) {
            if (parent->left == remove) {
                parent->left = leftMost;
            }
            else {
                parent->right = leftMost;
            }
        }
        else {
            this->root = leftMost;
        }
        leftMostParent = nullptr;
        remove->left = remove->right = nullptr;
        delete remove;
    }

    //1 child
    else if (remove->getLeft() || remove->getRight()) {
        BSTNode<Base> *child = nullptr;

        if(remove->getLeft()) {
            child = remove->left;
            remove->left = nullptr;
        }
        else {
            child = remove->right;
            remove->right = nullptr;
        }

        if (parent) {
            if (parent->getLeft() == remove) {
                parent->left = child;
            }
            else {
                parent->right = child;
            }
        }
        else {
            this->root = child;
        }
        delete remove;
    }
    else {
        if (parent) {
            if(parent->getLeft() == remove) {
                parent->left = nullptr;
            }
            else {
                parent->right = nullptr;
            }
        }
        else {
            this->root = nullptr;
        }
        delete remove;
    }
}

#endif