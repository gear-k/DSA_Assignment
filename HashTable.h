#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <iostream>   // For optional debug prints
#include <cstring>    // For strcmp, etc.
#include <functional> // For std::function

/***************************************************************************
 * Actor.h
 *
 * Team:            Gearoid, Cedric
 * Group:           1
 * Student IDs:     S10241866, S10241549
 *
 * Features Highlight:
 *   - Represents an actor with a fixed-size name buffer, birth year, age, and rating.
 *   - Provides safe copying and assignment operations.
 *   - Includes functions to get and set actor properties as well as display details.
 *
 ***************************************************************************/

// A function template to get the integer key from an item.
// This function must be specialized for each specific type.
template<typename T>
int getKey(const T& item) {
    static_assert(sizeof(T) == 0, "getKey must be specialized for each type.");
    return 0; // Placeholder; compiler forces specialization
}

// A simple separate-chaining hash table using linked-list chaining to resolve collisions.
// The table does not resize dynamically, so the initial size should be chosen carefully.
template <typename T>
class HashTable {
private:
    // Structure for nodes in the hash table's linked list.
    struct Node {
        T data;      // The item stored in the node
        Node* next;  // Pointer to the next node in the list

        // Constructor to initialize a node with the given data
        Node(const T& d);
    };

    Node** table;    // Array of pointers to nodes (buckets)
    int capacity;    // Number of buckets in the hash table
    int count;       // Total number of stored items

    // Computes the hash index for a given key using modulo operation.
    int hashFunc(int key) const;

public:
    // Constructor that initializes the hash table with a specified number of buckets.
    HashTable(int tableSize = 2000);

    // Destructor that clears and deallocates all hash table data.
    ~HashTable();

    // Copy constructor and assignment operator are disabled for simplicity.
    HashTable(const HashTable&) = delete;
    HashTable& operator=(const HashTable&) = delete;

    // Inserts an item into the hash table. If the key already exists, it updates the item.
    void insert(const T& item);

    // Removes an item from the hash table based on the key. Returns true if successful.
    bool remove(int key);

    // Finds an item by its key and returns a pointer to it, or nullptr if not found.
    T* find(int key) const;

    // Clears all items from the hash table, resetting the count.
    void clear();

    // Checks if the hash table is empty.
    bool isEmpty() const;

    // Returns the number of stored items.
    int size() const;

    // Iterates over all items in the table and applies a given function to each.
    // The function should return true if early termination is desired.
    void forEach(const std::function<bool(const T&)>& fn) const;

    // Retrieves the total number of stored items.
    int getCount() const;
};

#endif // HASHTABLE_H
