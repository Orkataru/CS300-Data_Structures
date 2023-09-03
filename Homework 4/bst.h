#ifndef BST_H
#define BST_H
#include <string>
#include <iostream>
#include "strutils.h"
#include <iomanip>
#include <fstream>
using namespace std;


template <class t>
struct Node {
    int height;
    t name, surname, phone, city, key;  // the data in the node
    Node<t>* left;      // pointer to the left child node
    Node<t>* right;     // pointer to the right child node
    Node<t>* parent;     // pointer to the parent node
    Node<t>() : left(nullptr), right(nullptr), parent(nullptr), height(1) {};
    Node<t>(t name, t surname, t phone, t city) : left(NULL), right(NULL), parent(NULL), height(1), name(name), surname(surname), city(city), phone(phone), key(UpperString(name + " " + surname)) {};
    ~Node<t>() {
        left = NULL;
        right = NULL;
        parent = NULL;
    }
    bool startsWith(Node<t> node) const {

        string phrase = LowerString(node.name);
        int len = phrase.length();
        if (LowerString(name.substr(0, len)) == LowerString(phrase)) {  // if the name matches, check the surname
            if (node.surname == "") return true;
            return false;
        }
        return false;
    }
};



template <class T>
class BST
{
public:
    Node<T>* root;

    BST();      // constructor
    BST(const BST<T>& rhs);    // copy constructor
    const BST<T>& operator=(const BST<T>& rhs);   // deep copy operator
    ~BST() {
        DeleteBST(root);
    }



    bool SearchAContact(const T& input) const;
    void InOrderPrintToFile(std::ofstream& file) const;
    void PreOrderPrintToFile(std::ofstream& file) const;
    void DrawTreeToFile(std::ofstream& file, Node<T>* node, int) const;
    bool AddContact(const T&, const T&, const T&, const T&);
    bool is_balanced(Node<T>* node);
    int left_h();
    int right_h();
    int balanceFactor(Node<T>*& node);
    int height(Node<T>* node);


    // takes full name of the contact to be deleted as an argument,
    // deletes the contact from the tree
    bool DeleteContact(const T&, const T&, Node<T>*&);

private:
    void SearchHelper(Node<T>*, const T&, bool&) const;
    void copyNodes(Node<T>* dest, Node<T>* src);
    void remove(const T&, const T&, Node<T>*&, bool&);
    void DeleteBST(Node<T>*&);
    void inorderHelper(Node<T>* node, std::ofstream& file)const;
    void preorderHelper(Node<T>* node, std::ofstream& file)const;
    Node<T>* minValueNode(Node<T>* node);

};


template<class T>
bool BST<T>::is_balanced(Node<T>* node) {

    return !(balanceFactor(node) > 1 || balanceFactor(node) < -1);
}


template<class T>
int BST<T>::height(Node<T>* node) {
    if (node == nullptr) {
        return 0;
    }
    return node->height = max(height(node->left), height(node->right)) + 1;
}


template<class T>
int BST<T>::balanceFactor(Node<T>*& node) {
    if (node == nullptr) {
        return 0;
    }
    return height(node->left) - height(node->right);
}


// CPP


template<class T>
int BST<T>::left_h() {
    Node<T>* current = root;
    int val = 0;
    while (current)
    {
        current = current->left;
        val++;
    }
    return val;
}
template<class T>
int BST<T>::right_h() {
    Node<T>* current = root;
    int val = 0;
    while (current)
    {
        current = current->right;
        val++;
    }
    return val;
}



template <class T>
bool BST<T>::DeleteContact(const T& name, const T& surname, Node<T>*& node)
{
    bool is_success = false;
    remove(name, surname, node, is_success);
    return is_success;
}

// private helper deletion function
template <class T>
void BST<T>::remove(const T& x, const T& surname, Node<T>*& t, bool& is_success)
{
    string key = UpperString(x + " " + surname);
    if (t == NULL)
        return;   // end of tree
    if (key < t->key)
        remove(x, surname, t->left, is_success);
    else if (t->key < key)
        remove(x, surname, t->right, is_success);
    else if (t->left != NULL && t->right != NULL) // Two children
    {
        Node<T>* inorder_successor = minValueNode(t->right);
        t->name = inorder_successor->name;
        t->surname = inorder_successor->surname;
        t->phone = inorder_successor->phone;
        t->city = inorder_successor->city;

        remove(t->name, t->surname, t->right, is_success);
    }
    else // one or no children
    {
        is_success = true;
        Node<T>* oldNode = t;
        t = (t->left != NULL) ? t->left : t->right;
        delete oldNode;

    }
}


template<class T>
Node<T>* BST<T>::minValueNode(Node<T>* node) {
    Node<T>* current = node;
    while (current && current->left != nullptr) {
        current = current->left;
    }
    return current;
}

template<class T>
bool BST<T>::AddContact(const T& name, const T& surname, const T& phone, const T& city) {
    if (name == "" || surname == "") return false;

    Node<T>* node = new Node<T>(name, surname, phone, city);
    Node<T>* current_node = root;
    while (true)
    {
        if (!current_node)
        {
            root = node;
            return true;
        }
        else if (UpperString(current_node->name) < UpperString(node->name)) {
            if (current_node->right)
            {
                current_node = current_node->right;
            }
            // leaf
            else
            {
                current_node->right = node;
                node->parent = current_node;
                return true;
            }
        }
        else if (UpperString(current_node->name) > UpperString(node->name)) {
            if (current_node->left)
            {
                current_node = current_node->left;
            }
            // leaf
            else {
                current_node->left = node;
                node->parent = current_node;
                return true;
            }
        }

        else {  // current_node->name == node->name

            if (UpperString(current_node->surname) < UpperString(node->surname)) {
                if (current_node->right)
                {
                    current_node = current_node->right;
                }
                // leaf
                else
                {
                    current_node->right = node;
                    node->parent = current_node;
                    return true;
                }
            }
            else if (UpperString(current_node->surname) > UpperString(node->surname)) {
                if (current_node->left)
                {
                    current_node = current_node->left;
                }
                // leaf
                else {
                    current_node->left = node;
                    node->parent = current_node;
                    return true;
                }
            }
            else
            {
                delete node;
                return false;
            }
        }

    }
}

template<class T>
void BST<T>::DeleteBST(Node<T>*& root) {
    if (root != NULL) {
        DeleteBST(root->left);
        DeleteBST(root->right);
        delete root;
    }
}


// recursive helper function that searches for the given 
template<class T>
void BST<T>::SearchHelper(Node<T>* current, const T& input, bool& isfound) const {
    if (current == NULL) return;

    if (UpperString(current->key).find(UpperString(input)) == 0)
    {

        cout << UpperString(current->name) << " "
            << UpperString(current->surname) << " "
            << current->phone << " "
            << current->city << endl;
        isfound = true;
        return;
    }

    SearchHelper(current->left, input, isfound);
    SearchHelper(current->right, input, isfound);

}

template<class T>
bool BST<T>::SearchAContact(const T& input) const {
    cout << "Searching an item in the phonebook (BST) . . ." << endl << endl;
    cout << "Phonebook: Searching for: (" << UpperString(input) << ")" << endl;
    cout << "====================================" << endl;
    bool isfound = false;
    SearchHelper(this->root, input, isfound);
    return isfound;
}


template<class T>
void BST<T>::inorderHelper(Node<T>* node, std::ofstream& file) const {
    if (!node) return;

    inorderHelper(node->left, file);

    file << UpperString(node->name) << " "
        << UpperString(node->surname) << " "
        << node->phone << " "
        << node->city << endl;

    inorderHelper(node->right, file);
}

template<class T>
void BST<T>::preorderHelper(Node<T>* node, std::ofstream& file) const {
    if (!node) return;

    file << UpperString(node->name) << " "
        << UpperString(node->surname) << " "
        << node->phone << " "
        << node->city << endl;

    preorderHelper(node->left, file);
    preorderHelper(node->right, file);
}

template<class T>
void BST<T>::InOrderPrintToFile(std::ofstream& file) const {
    Node<T>* node = root;
    inorderHelper(node, file);
}

template<class T>
void BST<T>::PreOrderPrintToFile(std::ofstream& file) const {
    Node<T>* node = root;
    preorderHelper(node, file);
}

template<class T>
const BST<T>& BST<T>::operator=(const BST<T>& rhs) {
    if (this == &rhs) {
        return *this;
    }
    DeleteBST(this->root);
    if (rhs.root == nullptr) {
        return *this;
    }
    root = new Node<T>(rhs.root->name, rhs.root->surname, rhs.root->phone, rhs.root->city);
    copyNodes(root, rhs.root);
    return *this;
}


template<class T>
void BST<T>::copyNodes(Node<T>* dest, Node<T>* src) {
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
BST<T>::BST(const BST<T>& other) : root(nullptr) {
    *this = other;
}

template<class T>
BST<T>::BST() : root(nullptr) {
}


template<class T>
void BST<T>::DrawTreeToFile(std::ofstream& file, Node<T>* root, int depth) const {
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

#endif