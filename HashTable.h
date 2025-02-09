#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <iostream>   // For optional debug prints
#include <cstring>    // For strcmp, etc.
#include <functional> // For std::function

/***************************************************************************
 * HashTable.h
 *
 * Team:            Gearoid, Cedric
 * Group:           1
 * Student IDs:     S10241866, S10241549
 *
 * Features Highlight:
 *   - Implements a simple separate-chaining hash table using linked-list chaining.
 *   - The table does not resize dynamically, so the initial size should be chosen carefully.
 *   - Provides basic operations such as insert, remove, find, and iteration.
 *
 ***************************************************************************/

 /**
  * @brief Template function to retrieve an integer key from an item.
  *
  * This function must be specialized for each type T.
  *
  * @tparam T The type of the item.
  * @param item The item from which to extract the key.
  * @return int The key associated with the item.
  */
template<typename T>
int getKey(const T& item) {
    static_assert(sizeof(T) == 0, "getKey must be specialized for each type.");
    return 0; // Placeholder; compiler forces specialization
}

/**
 * @brief A simple separate-chaining hash table using linked-list chaining.
 *
 * This hash table does not resize dynamically, so choose the initial table size carefully.
 *
 * @tparam T The type of elements stored in the hash table.
 */
template <typename T>
class HashTable {
private:
    /**
     * @brief Node structure for storing items in the hash table.
     */
    struct Node {
        T data;      ///< The item stored in the node.
        Node* next;  ///< Pointer to the next node in the list.

        /**
         * @brief Constructs a Node with the provided data.
         *
         * @param d The data to store in the node.
         */
        Node(const T& d);
    };

    Node** table;    ///< Array of pointers to nodes (buckets).
    int capacity;    ///< Number of buckets in the hash table.
    int count;       ///< Total number of stored items.

    /**
     * @brief Computes the hash index for a given key.
     *
     * Uses a simple modulo operation on the key.
     *
     * @param key The key for which to compute the hash.
     * @return int The index in the hash table.
     */
    int hashFunc(int key) const;

public:
    /**
     * @brief Constructs a HashTable with a specified number of buckets.
     *
     * @param tableSize The number of buckets in the hash table (default is 2000).
     */
    HashTable(int tableSize = 2000);

    /**
     * @brief Destructor for the HashTable.
     *
     * Clears and deallocates all hash table data.
     */
    ~HashTable();

    // Copy constructor and assignment operator are disabled for simplicity.
    HashTable(const HashTable&) = delete;
    HashTable& operator=(const HashTable&) = delete;

    /**
     * @brief Inserts an item into the hash table.
     *
     * If an item with the same key already exists, it updates the item.
     *
     * @param item The item to insert.
     */
    void insert(const T& item);

    /**
     * @brief Removes an item from the hash table based on its key.
     *
     * @param key The key of the item to remove.
     * @return true if the item was successfully removed, false otherwise.
     */
    bool remove(int key);

    /**
     * @brief Finds an item in the hash table by its key.
     *
     * @param key The key of the item to find.
     * @return A pointer to the found item, or nullptr if not found.
     */
    T* find(int key) const;

    /**
     * @brief Clears all items from the hash table.
     *
     * Resets the count to zero.
     */
    void clear();

    /**
     * @brief Checks whether the hash table is empty.
     *
     * @return true if the hash table contains no items, false otherwise.
     */
    bool isEmpty() const;

    /**
     * @brief Returns the number of stored items in the hash table.
     *
     * @return int The number of items.
     */
    int size() const;

    /**
     * @brief Iterates over all items in the hash table and applies a given function.
     *
     * The function is applied to each item and should return true if early termination is desired.
     *
     * @param fn A function to apply to each item.
     */
    void forEach(const std::function<bool(const T&)>& fn) const;

    /**
     * @brief Retrieves the total number of stored items.
     *
     * @return int The total count of items.
     */
    int getCount() const;
};

#endif // HASHTABLE_H
