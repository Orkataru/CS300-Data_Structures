#pragma once
#include <string>
#include <vector>
#include <list>

using namespace std;

class Contact {
public:
    string name, surname, phone, city, key;
    Contact(string name, string surname, string phone, string city) : name(name), surname(surname), phone(phone), city(city), key(name + " " + surname) {}
    Contact(const Contact& old_contact) {
        name = old_contact.name;
        surname = old_contact.surname;
        phone = old_contact.phone;
        city = old_contact.city;
        key = old_contact.key;
    }

    bool operator==(const Contact& rhs) const{
        return (rhs.name == name && rhs.surname == surname);
    }

    Contact& operator=(const Contact& rhs) {
        if (this == &rhs) return *this;
        name = rhs.name;
        surname = rhs.surname;
        phone = rhs.phone;
        city = rhs.city;
        key = rhs.key;
        return *this;
    }
};


template <typename KeyType, typename ValueType>
class HashTable
{
public:
    int _TABLE_SIZE;
    double load_factor;
    HashTable(size_t size) : _TABLE_SIZE(size) {
        thelists.resize(_TABLE_SIZE);

    }
    HashTable(const HashTable& table);

    HashTable& operator=(const HashTable& rhs) {
        thelists = rhs.thelists;
        return *this;
    }

    bool DeleteContact(const KeyType);
    bool insert(const KeyType& key, const ValueType& val);
    bool lookup(const KeyType&) const;
    vector<list<ValueType> > rehash();
    vector<list<ValueType> > thelists; // The vector of lists

};

int hashF (string key, int _TABLE_SIZE)
{
    key = UpperString(key);
    int hashVal = 0;
    for (char c : key) {
        hashVal += c;
    }

    return hashVal % _TABLE_SIZE;
}

int hashF (int key, int _TABLE_SIZE)
{
    return  (key % _TABLE_SIZE);
}

template <typename KeyType, typename ValueType>
vector<list<ValueType>> HashTable<KeyType, ValueType>::rehash() {
    int prev_size = _TABLE_SIZE;
    _TABLE_SIZE = _TABLE_SIZE*2 + 1;
    HashTable<KeyType, ValueType> new_table(_TABLE_SIZE);
    for (int i = 0; i < thelists.size(); i++) {
        for (auto& val : thelists[i]) {
            string key = val.key;
            int idx = hashF(key, _TABLE_SIZE);
            new_table.thelists[idx].push_back(val);
        }
    }

    // calculate load
    double w_count = 0;
    for (size_t i = 0; i < thelists.size(); i++)
    {
        w_count += thelists[i].size();

    }

    // calculate load factor
    load_factor = double(w_count / _TABLE_SIZE);


    cout << "rehashed..." << endl;
    cout << "previous table size: " << prev_size << ", new table size : " << _TABLE_SIZE << ", current unique word count " << w_count << ", current load factor " << load_factor << endl;

    return new_table.thelists;
}

template <typename KeyType, typename ValueType>
HashTable<KeyType, ValueType>::HashTable(const HashTable& table) {
    *this = table;
}

template <typename KeyType, typename ValueType>
bool HashTable<KeyType, ValueType>::insert(const KeyType& key, const ValueType& val)
{
    int idx = hashF(key, _TABLE_SIZE);

    // Insert the value if it does not already exist in the list, skip otherwise
    for (auto& el : thelists[idx])
    {
        if (el == val) return false;  // exit the function if value already exists
    }

    thelists[idx].push_back(val);

    // calculate word count
    double w_count = 0;
    for (size_t i = 0; i < thelists.size(); i++)
    {
        w_count += thelists[i].size();

    }

    // update the load factor
    load_factor = (w_count / _TABLE_SIZE);

    if (load_factor > 0.7)
    {
        thelists = rehash();
    }
    return true;
}

template <typename KeyType, typename ValueType>
bool HashTable<KeyType, ValueType>::lookup(const KeyType& key) const {

    cout << "\nSearching an item in the phonebook (HashTable) . . ." << endl;
    cout << "Phonebook: Searching for: (" << UpperString(key) << ")" << endl;
    cout << "====================================" << endl;

    int hashValue = hashF(key, _TABLE_SIZE);
    int index = hashValue % _TABLE_SIZE;

    for (auto& contact : thelists[index]) {

        if (key == contact.key) { // found   
            cout << UpperString(contact.name) << " "
                << UpperString(contact.surname) << " "
                << contact.phone << " "
                << contact.city << endl;

            return true;
        }
    }
    return false;
}


template <typename KeyType, typename ValueType>
bool HashTable<KeyType, ValueType>::DeleteContact(const KeyType key) {
    int hashValue = hashF(key, _TABLE_SIZE);

    for (const auto contact : thelists[hashValue]) {

        if (key == contact.key) { // found   
            thelists[hashValue].remove(contact);
            return true;
        }
    }

    return false;

}