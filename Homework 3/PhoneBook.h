#pragma once
#include "avl.h"
#include "bst.h"
#include <string>
#include <iostream>
#include <sstream>
#include <chrono>
#include "strutils.h"
#include <fstream>

template <class T>
class PhoneBook {
private:
public:
	BST<T> bst;
	AVL<T> avl;
	PhoneBook();

	void SearchAContact(const T& input) const;
	void InOrderPrintToFile() const;
	void PreOrderPrintToFile() const;
	void DrawTreeToFile(std::ofstream& file_bst, std::ofstream& file_avl) const;
	void AddContact(const T&, const T&, const T&, const T&);
	void DeleteContact(const T&, const T&);
	void LoadtoBST(string&);
	void LoadtoAVL(string&);
};



//CPP


template<class T>
void PhoneBook<T>::LoadtoBST(string& filename) {

	cout << "Loading the phonebook into a BST." << endl;
	ifstream file_in(filename);
	auto start = std::chrono::high_resolution_clock::now();	// start timer 
	if (file_in.is_open())
	{
		do
		{	
			string name, surname, phone, city, line;
			getline(file_in, line);
			stringstream s(line);



			s >> name;	// name
			s >> surname;	// surname
			s >> phone;	// phone number
			s >> city;	// city


			bst.AddContact(name, surname, phone, city);
		} while (!file_in.eof());
	}
	auto finish = std::chrono::high_resolution_clock::now();
	std::cout << "Phonebook creation in BST took " << std::chrono::duration_cast<std::chrono::milliseconds>(finish - start).count() << " milliseconds. . .\n";


	if (bst.is_balanced(bst.root))
	{
		cout << "The tree is balanced" << endl;
	}
	else
	{
		cout << "The tree is not balanced" << endl;
	}

	cout << "The heights of BST are for left: " << bst.height(bst.root->left) << " and right: " << bst.height(bst.root->right) << endl;

}


template<class T>
void PhoneBook<T>::LoadtoAVL(string& filename) {
	cout << "Loading the phonebook into an AVL." << endl;
	ifstream file_in(filename);
	auto start = std::chrono::high_resolution_clock::now();	// start timer 
	if (file_in.is_open())
	{
		do
		{
			string name, surname, phone, city;

			file_in >> name;	// name
			file_in >> surname;	// surname
			file_in >> phone;	// phone number
			file_in >> city;	// city


			avl.AddContact(name, surname, phone, city);
		} while (!file_in.eof());
		file_in.close();
	}
	auto finish = std::chrono::high_resolution_clock::now();
	std::cout << "Phonebook creation in AVL took " << std::chrono::duration_cast<std::chrono::milliseconds>(finish - start).count() << " milliseconds. . .\n";


	if (!(avl.getBalance(avl.root) > 1 || avl.getBalance(avl.root) < -1))
	{
		cout << "The tree is balanced" << endl;
	}
	else
	{
		cout << "The tree is not balanced" << endl;
	}

	cout << "The heights of AVL are for left: " << avl.getHeight(avl.root->left) << " and right: " << avl.getHeight(avl.root->right) << endl;
}



template<class T>
PhoneBook<T>::PhoneBook() {}


template<class T>
void PhoneBook<T>::SearchAContact(const T& input) const {
	long unsigned times[2];

	auto start_bst = std::chrono::high_resolution_clock::now();	// start timer 
	bst.SearchAContact(input);
	auto finish_bst = std::chrono::high_resolution_clock::now();	// start timer 
	times[0] = std::chrono::duration_cast<std::chrono::nanoseconds>(finish_bst - start_bst).count();

	auto start_avl = std::chrono::high_resolution_clock::now();	// start timer 
	avl.SearchAContact(input);
	auto finish_avl = std::chrono::high_resolution_clock::now();	// start timer 
	times[1] = std::chrono::duration_cast<std::chrono::nanoseconds>(finish_avl - start_avl).count();

	cout << "The search in BST took " << times[0] << " nanoseconds. . ." << endl;
	cout << "The search in AVL took " << times[1] << " nanoseconds. . ." << endl;
	cout << "\n";

}

template<class T>
void PhoneBook<T>::InOrderPrintToFile() const {
	long unsigned times[2];
	ofstream bst_out("phonebookInOrderBST.txt");
	ofstream avl_out("phonebookInOrderAVL.txt");
	auto start_bst = std::chrono::high_resolution_clock::now();	// start timer 
	bst.InOrderPrintToFile(bst_out);
	auto finish_bst = std::chrono::high_resolution_clock::now();	// start timer 
	times[0] = std::chrono::duration_cast<std::chrono::nanoseconds>(finish_bst - start_bst).count();


	auto start_avl = std::chrono::high_resolution_clock::now();	// start timer 
	avl.InOrderPrintToFile(avl_out);
	auto finish_avl = std::chrono::high_resolution_clock::now();	// start timer 
	times[1] = std::chrono::duration_cast<std::chrono::nanoseconds>(finish_avl - start_avl).count();

	cout << "Printing in order to file from the Binary Tree took " << times[0] << " nanoseconds. . ." << endl;
	cout << "Printing in order to file from AVL Tree took " << times[1] << " nanoseconds. . ." << endl;



}

template<class T>
void PhoneBook<T>::PreOrderPrintToFile() const {
	long unsigned times[2];
	ofstream bst_out("phonebookPreOrderBST.txt");
	ofstream avl_out("phonebookPreOrderAVL.txt");
	auto start_bst = std::chrono::high_resolution_clock::now();	// start timer 
	bst.PreOrderPrintToFile(bst_out);
	auto finish_bst = std::chrono::high_resolution_clock::now();	// stop timer 
	times[0] = std::chrono::duration_cast<std::chrono::nanoseconds>(finish_bst - start_bst).count();


	auto start_avl = std::chrono::high_resolution_clock::now();	// start timer 
	avl.PreOrderPrintToFile(avl_out);
	auto finish_avl = std::chrono::high_resolution_clock::now();	// stop timer 
	times[1] = std::chrono::duration_cast<std::chrono::nanoseconds>(finish_avl - start_avl).count();

	cout << "Printing in order to file from the Binary Tree took " << times[0] << " nanoseconds. . ." << endl;
	cout << "Printing in order to file from AVL Tree took " << times[1] << " nanoseconds. . ." << endl;
	cout << "\n";
}

template<class T>
void PhoneBook<T>::AddContact(const T& name, const T& surname, const T& phone, const T& city) {
	long unsigned times[2];
	bool is_added_BST, is_added_AVL;
	cout << "Adding an item to the phonebook (BST) . . ." << endl;
	cout << "====================================" << endl;
	auto start_bst = std::chrono::high_resolution_clock::now();	// start timer 
	is_added_BST = bst.AddContact(name, surname, phone, city);
	if(is_added_BST)	cout << "Contact has been added successfully to the BST" << endl << endl;
	else cout << "The given contact full name already exists in the database." << endl << endl;
	auto finish_bst = std::chrono::high_resolution_clock::now();	// stop timer 
	times[0] = std::chrono::duration_cast<std::chrono::nanoseconds>(finish_bst - start_bst).count();


	cout << "Adding an item to the phonebook (AVL) . . ." << endl;
	cout << "====================================" << endl;
	auto start_avl = std::chrono::high_resolution_clock::now();	// start timer 
	is_added_AVL = avl.AddContact(name, surname, phone, city);
	if (is_added_AVL)	cout << "Contact has been added successfully to the AVL" << endl << endl;
	else cout << "The given contact full name already exists in the database." << endl << endl;
	auto finish_avl = std::chrono::high_resolution_clock::now();	// stop timer 
	times[1] = std::chrono::duration_cast<std::chrono::nanoseconds>(finish_avl - start_avl).count();


	if (is_added_BST) cout << "Adding a contact to the Binary Tree took " << times[0] << " nanoseconds. . ." << endl;
	if (is_added_AVL) cout << "Adding a contact to the AVL Tree took " << times[1] <<  " nanoseconds. . ." << endl;
	cout << "\n";
}

template<class T>
void PhoneBook<T>::DeleteContact(const T& name, const T& surname) {

	long unsigned times[2];
	bool is_deleted_BST, is_deleted_AVL;

	auto start_bst = std::chrono::high_resolution_clock::now();	// start timer 
	is_deleted_BST = bst.DeleteContact(name, surname, bst.root);
	auto finish_bst = std::chrono::high_resolution_clock::now();	// stop timer 
	times[0] = std::chrono::duration_cast<std::chrono::nanoseconds>(finish_bst - start_bst).count();


	auto start_avl = std::chrono::high_resolution_clock::now();	// start timer 
	is_deleted_AVL = avl.DeleteContact(name, surname, avl.root);
	auto finish_avl = std::chrono::high_resolution_clock::now();	// stop timer 
	times[1] = std::chrono::duration_cast<std::chrono::nanoseconds>(finish_avl - start_avl).count();

	if (is_deleted_BST)	cout << "Deleted successfully. . ." << endl << endl;
	else cout << "\nNot found" << endl << endl;
	if (is_deleted_BST) cout << "Deletion from the Binary Tree took " << times[0] << " nanoseconds. . ." << endl;
	if (is_deleted_AVL) cout << "Deletion from AVL Tree took " << times[1] << " nanoseconds. . ." << endl;
	cout << "\n";

}

template<class T>
void PhoneBook<T>::DrawTreeToFile(std::ofstream& file_bst, std::ofstream& file_avl) const {
	long unsigned times[2];
	auto start_bst = std::chrono::high_resolution_clock::now();	// start timer 
	bst.DrawTreeToFile(file_bst, bst.root, 0);
	auto finish_bst = std::chrono::high_resolution_clock::now();	// start timer 
	times[0] = std::chrono::duration_cast<std::chrono::nanoseconds>(finish_bst - start_bst).count();


	auto start_avl = std::chrono::high_resolution_clock::now();	// start timer 
	avl.DrawTreeToFile(file_avl, avl.root, 0);
	auto finish_avl = std::chrono::high_resolution_clock::now();	// start timer 
	times[1] = std::chrono::duration_cast<std::chrono::nanoseconds>(finish_avl - start_avl).count();

	cout << "Drawing tree to file from the Binary Tree took " << times[0] << " nanoseconds. . ." << endl;
	cout << "Drawing tree to file from AVL Tree took " << times[1] << " nanoseconds. . ." << endl;
	cout << "\n";
}
