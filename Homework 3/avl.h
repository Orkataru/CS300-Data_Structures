#pragma once
#include "bst.h"
#include "strutils.h"
#include <iostream>
#include <iomanip>
#include <string>
template<class T>
class AVL 
{
public:
	Node<T>* root;
	AVL();      // constructor
	AVL(const AVL<T>& rhs);    // copy constructor
	const AVL<T>& operator=(const AVL<T>& rhs);   // deep copy operator
	bool AddContact(const T& name, const T& surname, const T& phone, const T& city) ;
	bool DeleteContact(const T&, const T&, Node<T>*&) ;

    void SearchAContact(const T& input) const;
    void InOrderPrintToFile(std::ofstream& file) const;
    void PreOrderPrintToFile(std::ofstream& file) const;
    void DrawTreeToFile(std::ofstream& file, Node<T>*, int) const;

	~AVL() {
		DeleteAVL(root);
	}

    // GPT

    int getHeight(Node<T>* n);
    int getBalance(Node<T>* n);
    Node<T>* rotateRight(Node<T>*& n);
    Node<T>* rotateLeft(Node<T>*& n);
    Node<T>* insert(Node<T>*& n, T name, T surname, T phone, T city, bool&);



private:
	void DeleteAVL(Node<T>*& root);


    void SearchHelper(Node<T>* start_node, const T& input) const;
    void copyNodes(Node<T>* dest, Node<T>* src);
    void remove(const T& x, const T& surname, Node<T>*& t, Node<T>*& root, bool&);
    void inorderHelper(Node<T>* node, std::ofstream& file)const;
    void preorderHelper(Node<T>* node, std::ofstream& file)const;
    Node<T>* minValueNode(Node<T>* node);






};

template<class T>
int AVL<T>::getHeight(Node<T>* n) {
    if (n == nullptr) {
        return 0;
    }
    return n->height;
}

template<class T>
int  AVL<T>::getBalance(Node<T>* n) {
    if (n == nullptr) {
        return 0;
    }
    return getHeight(n->left) - getHeight(n->right);
}

template<class T>
Node<T>* AVL<T>::rotateRight(Node<T>*& n) {
    Node<T>* newRoot = n->left;
    n->left = newRoot->right;
    newRoot->right = n;
    n->height = std::max(getHeight(n->left), getHeight(n->right)) + 1;
    newRoot->height = std::max(getHeight(newRoot->left), getHeight(newRoot->right)) + 1;
    return newRoot;
}

template<class T>
Node<T>* AVL<T>::rotateLeft(Node<T>*& n) {
    Node<T>* newRoot = n->right;
    n->right = newRoot->left;
    newRoot->left = n;
    n->height = std::max(getHeight(n->left), getHeight(n->right)) + 1;
    newRoot->height = std::max(getHeight(newRoot->left), getHeight(newRoot->right)) + 1;
    return newRoot;
}

template<class T>
Node<T>* AVL<T>::insert(Node<T>*& n, T name, T surname, T phone, T city, bool& is_success) {
    if (name == "" || surname == "") return n;
    T key = UpperString(name + " " + surname);
    if (n == nullptr) {
        is_success = true;
        return new Node<T>(name, surname, phone, city);
    }

    if (key < n->key) {
        n->left = insert(n->left, name, surname, phone, city, is_success);
    }
    else if (key > n->key) {
        n->right = insert(n->right, name, surname, phone, city, is_success);
    }
    else {
        is_success = false;
        return n;
    }

    n->height = std::max(getHeight(n->left), getHeight(n->right)) + 1;
    int balance = getBalance(n);

    if (balance > 1 && key < n->left->key) {
        return rotateRight(n);
    }

    if (balance < -1 && key > n->right->key) {
        return rotateLeft(n);
    }

    if (balance > 1 && key > n->left->key) {
        n->left = rotateLeft(n->left);
        return rotateRight(n);
    }

    if (balance < -1 && key < n->right->key) {
        n->right = rotateRight(n->right);
        return rotateLeft(n);
    }

    return n;
}


template<class T>
bool AVL<T>::AddContact(const T& name, const T& surname, const T& phone, const T& city) {
    bool is_success;
    root = insert(root, name, surname, phone, city, is_success);
    return is_success;
}

template <class T>
bool AVL<T>::DeleteContact(const T& name, const T& surname, Node<T>*& node)
{
    bool is_success = false;
    remove(name, surname, node, root, is_success);
    return is_success;

}



template<class T>
void AVL<T>::DeleteAVL(Node<T>*& root) {
    if (root != NULL) {
        DeleteAVL(root->left);
        DeleteAVL(root->right);
        delete root;
    }
}

template<class T>
const AVL<T>& AVL<T>::operator=(const AVL<T>& rhs) {
    if (this == &rhs) {
        return *this;
    }
    DeleteAVL(this->root);
    if (rhs.root == nullptr) {
        return *this;
    }
    root = new Node<T>(rhs.root->name, rhs.root->surname, rhs.root->phone, rhs.root->city);
    copyNodes(root, rhs.root);
    return *this;
}

template<class T>
AVL<T>::AVL(const AVL<T>& other) : root(nullptr) {
    *this = other;
}

template<class T>
AVL<T>::AVL() : root(nullptr) {
}


// private helper deletion function
template <class T>
void AVL<T>::remove(const T& x, const T& surname, Node<T>*& t, Node<T>*& root, bool& is_success)
{
    string key = UpperString(x + " " + surname);
    if (t == NULL)
        return;   // end of tree
    if (key < t->key)
        remove(x, surname, t->left, root, is_success);
    else if (t->key < key)
        remove(x, surname, t->right, root, is_success);
    else if (t->left != NULL && t->right != NULL) // Two children
    {
        Node<T>* inorder_successor = minValueNode(t->right);
        t->name = inorder_successor->name;
        t->surname = inorder_successor->surname;
        t->phone = inorder_successor->phone;
        t->city = inorder_successor->city;

        remove(t->name, t->surname, t->right, root, is_success);
    }
    else // one or no children
    {
        is_success = true;
        Node<T>* oldNode = t;
        t = (t->left != NULL) ? t->left : t->right;
        delete oldNode;

    }

    // Rebalance the AVL tree after deletion
    if (t != NULL) {
        t->height = 1 + max(getHeight(t->left), getHeight(t->right));
        int balanceFactor = getBalance(t);

        if (balanceFactor > 1 && getBalance(t->left) >= 0) {
            root = rotateRight(t);
        }
        else if (balanceFactor > 1 && getBalance(t->left) < 0) {
            t->left = rotateLeft(t->left);
            root = rotateRight(t);
        }
        else if (balanceFactor < -1 && getBalance(t->right) <= 0) {
            root = rotateLeft(t);
        }
        else if (balanceFactor < -1 && getBalance(t->right) > 0) {
            t->right = rotateRight(t->right);
            root = rotateLeft(t);
        }
    }
}


template<class T>
Node<T>* AVL<T>::minValueNode(Node<T>* node) {
    Node<T>* current = node;
    while (current && current->left != nullptr) {
        current = current->left;
    }
    return current;
}

// recursive helper function that searches for the given 
template<class T>
void AVL<T>::SearchHelper(Node<T>* current, const T& input) const {

    if (current == NULL) return;

    if (UpperString(current->key).find(UpperString(input)) == 0)
    {

        cout << UpperString(current->name) << " "
            << UpperString(current->surname) << " "
            << current->phone << " "
            << current->city << endl;
    }

    SearchHelper(current->left, input);
    SearchHelper(current->right, input);

}

template<class T>
void AVL<T>::SearchAContact(const T& input) const {
    cout << "Searching an item in the phonebook (AVL) . . ." << endl << endl;
    cout << "Phonebook: Searching for: (" << UpperString(input) << ")" << endl;
    cout << "====================================" << endl;
    SearchHelper(this->root, input);
    cout << endl;
}

template<class T>
void AVL<T>::inorderHelper(Node<T>* node, std::ofstream& file) const {
    if (!node) return;

    inorderHelper(node->left, file);

    file << UpperString(node->name) << " "
        << UpperString(node->surname) << " "
        << node->phone << " "
        << node->city << endl;

    inorderHelper(node->right, file);
}

template<class T>
void AVL<T>::preorderHelper(Node<T>* node, std::ofstream& file)const {
    if (!node) return;


    file << UpperString(node->name) << " "
        << UpperString(node->surname) << " "
        << node->phone << " "
        << node->city << endl;


    preorderHelper(node->left, file);
    preorderHelper(node->right, file);
}


template<class T>
void AVL<T>::InOrderPrintToFile(std::ofstream& file) const {
    Node<T>* node = root;
    inorderHelper(node, file);
}

template<class T>
void AVL<T>::PreOrderPrintToFile(std::ofstream& file) const {
    Node<T>* node = root;
    preorderHelper(node, file);
}


template<class T>
void AVL<T>::copyNodes(Node<T>* dest, Node<T>* src) {
    if (src == nullptr) {
        return;
    }
    if (src->left != nullptr) {
        dest->left = new Node<T>(src->left->data);
        copyNodes(dest->left, src->left);
    }
    if (src->right != nullptr) {
        dest->right = new Node<T>(src->right->data);
        copyNodes(dest->right, src->right);
    }
}


template<class T>
void AVL<T>::DrawTreeToFile(std::ofstream& file, Node<T>* root, int depth) const {
    if (root == NULL) {
        return;
    }
    for (int i = 0; i < depth; i++) {
        file << "|  ";
    }
    file << "|--" << UpperString(root->key) << endl;
    DrawTreeToFile(file, root->left, depth + 1);
    DrawTreeToFile(file, root->right, depth + 1);

}
