#pragma once
#include "bst.h"
#include "HashTable.h"
#include <string>
#include <iostream>
#include <sstream>
#include <chrono>
#include "strutils.h"
#include <fstream>

template <class T>
class PhoneBook {
public:
	HashTable<T, Contact> ht = HashTable<T, Contact>(53);
	BST<T> bst;


	PhoneBook<T>();

	void SearchAContact(const T& input) const;
	void DeleteContact(const T&, const T&);
	void LoadtoBST(string&);
	void LoadtoHT(string&);
	void AddContact(const T& name, const T& surname, const T& phone, const T& city);



 };

template<class T>
PhoneBook<T>::PhoneBook() {}

//CPP


template<class T>
void PhoneBook<T>::LoadtoBST(string& filename) {


	ifstream file_in(filename);
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
	
	
}

template<class T>
void PhoneBook<T>::LoadtoHT(string& filename) {
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
			if (name == "" || surname == "") continue;

			const Contact contact(name, surname, phone, city);
			ht.insert(contact.key, contact);
		} while (!file_in.eof());
		file_in.close();
	}
	
	// calculate load
	double w_count = 0;
	for (size_t i = 0; i < ht.thelists.size(); i++)
	{
		w_count+=ht.thelists[i].size();

	}

	// calculate load factor
	double load_factor = double(w_count / ht._TABLE_SIZE);

	cout << "\nAfter preprocessing, the unique word count is " << w_count << ". Current load ratio is " << load_factor;
}


template<class T>
void PhoneBook<T>::SearchAContact(const T& input) const {
	double times[2];
	
	bool isfound = false;
	auto start_bst = std::chrono::high_resolution_clock::now();	// start timer 
	isfound = bst.SearchAContact(input);
	auto finish_bst = std::chrono::high_resolution_clock::now();	// start timer 
	times[0] = std::chrono::duration_cast<std::chrono::nanoseconds>(finish_bst - start_bst).count();

	if (!isfound) cout << "Name not found!\n" << endl;

	auto start_ht = std::chrono::high_resolution_clock::now();	// start timer 
	isfound = ht.lookup(input);
	auto finish_ht = std::chrono::high_resolution_clock::now();	// start timer 
	times[1] = std::chrono::duration_cast<std::chrono::nanoseconds>(finish_ht - start_ht).count();

	if (!isfound) cout << "Name not found!" << endl;

	if (isfound)
	{
		cout << "\nBST Search Time: " << times[0] << endl;
		cout << "Hash Table Search Time: " << times[1] << endl;
		cout << "Speed Up: " << times[0]/times[1];
		cout << "\n";
	}

}



template<class T>
void PhoneBook<T>::DeleteContact(const T& name, const T& surname) {

	long unsigned times[2];
	bool is_deleted_BST, is_deleted_HT;

	auto start_bst = std::chrono::high_resolution_clock::now();	// start timer 
	is_deleted_BST = bst.DeleteContact(name, surname, bst.root);
	auto finish_bst = std::chrono::high_resolution_clock::now();	// stop timer 
	times[0] = std::chrono::duration_cast<std::chrono::nanoseconds>(finish_bst - start_bst).count();

	auto start_ht = std::chrono::high_resolution_clock::now();	// start timer 
	is_deleted_HT = ht.DeleteContact(name + " " + surname);
	auto finish_ht = std::chrono::high_resolution_clock::now();	// stop timer 
	times[1] = std::chrono::duration_cast<std::chrono::nanoseconds>(finish_ht - start_ht).count();

	if (is_deleted_BST)	cout << "Deleted successfully. . ." << endl << endl;
	else cout << "\nNot found" << endl << endl;
	if (is_deleted_BST) cout << "Deletion from the Binary Tree took " << times[0] << " nanoseconds. . ." << endl;
	if (is_deleted_HT) cout << "Deletion from Hashtable Tree took " << times[1] << " nanoseconds. . ." << endl;
	cout << "\n";

}


template<class T>
void PhoneBook<T>::AddContact(const T& name, const T& surname, const T& phone, const T& city) {
	long unsigned times[2];
	bool is_added_BST, is_added_HT;
	cout << "Adding an item to the phonebook (BST) . . ." << endl;
	cout << "====================================" << endl;
	auto start_bst = std::chrono::high_resolution_clock::now();	// start timer 
	is_added_BST = bst.AddContact(name, surname, phone, city);
	if (is_added_BST)	cout << "Contact has been added successfully to the BST" << endl << endl;
	else cout << "The given contact full name already exists in the database." << endl << endl;
	auto finish_bst = std::chrono::high_resolution_clock::now();	// stop timer 
	times[0] = std::chrono::duration_cast<std::chrono::nanoseconds>(finish_bst - start_bst).count();


	cout << "Adding an item to the phonebook (HashTable) . . ." << endl;
	cout << "====================================" << endl;
	Contact con(name, surname, phone, city);
	auto start_ht = std::chrono::high_resolution_clock::now();	// start timer 
	is_added_HT = ht.insert(name + " " + surname, con);
	if (is_added_HT)	cout << "Contact has been added successfully to the Hastable" << endl << endl;
	else cout << "The given contact full name already exists in the database." << endl << endl;
	auto finish_ht = std::chrono::high_resolution_clock::now();	// stop timer 
	times[1] = std::chrono::duration_cast<std::chrono::nanoseconds>(finish_ht - start_ht).count();


	if (is_added_BST) cout << "Adding a contact to the Binary Tree took " << times[0] << " nanoseconds. . ." << endl;
	if (is_added_HT) cout << "Adding a contact to the Hashtable took " << times[1] << " nanoseconds. . ." << endl;
	cout << "\n";
}
