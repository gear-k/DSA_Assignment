#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <iostream>  // For optional debug prints
#include <cstring>   // For strcmp, etc.

/***************************************************************************
 * HashTable.h
 *
 * Team:            Gearoid, Cedric
 * Group:           1
 * Student IDs:     S10241866, S10241549
 *
 * Features Highlight:
 *   - Implements a simple separate-chaining hash table.
 *   - Uses linked-list chaining to handle key collisions.
 *   - Assumes type T has a unique integer key accessible via getKey(T).
 *
 ***************************************************************************/

 /**
  * @brief Function template to retrieve the integer key from an item of type T.
  *
  * This function must be specialized for each type stored in the hash table.
  *
  * @tparam T The type of the item.
  * @param item The item from which to retrieve the key.
  * @return The unique integer key associated with the item.
  */
template<typename T>
int getKey(const T& item) {
    static_assert(sizeof(T) == 0, "getKey must be specialized for each type.");
    return 0; // Placeholder; compiler forces specialization
}

/**
 * @brief A simple separate-chaining hash table.
 *
 * This hash table uses linked-list chaining to resolve key collisions.
 * It is not dynamically resized, so an appropriate bucket count should be chosen.
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
        T data;      ///< The data stored in this node.
        Node* next;  ///< Pointer to the next node in the chain.

        /**
         * @brief Constructs a new Node with the given data.
         *
         * @param d The data to store in the node.
         */
        Node(const T& d) : data(d), next(nullptr) {}
    };

    Node** table;    ///< Array of pointers to nodes (the buckets).
    int capacity;    ///< Number of buckets in the hash table.
    int count;       ///< Total number of items stored in the table.

    /**
     * @brief Computes the hash index for a given key.
     *
     * The hash function computes the key modulo the capacity and ensures a
     * non-negative result.
     *
     * @param key The key to hash.
     * @return The bucket index.
     */
    int hashFunc(int key) const {
        return (key % capacity + capacity) % capacity;
    }

public:
    /**
     * @brief Constructs a new HashTable object.
     *
     * Initializes the hash table with a given number of buckets.
     *
     * @param tableSize The number of buckets to allocate (default is 2000).
     */
    HashTable(int tableSize = 2000)
        : capacity(tableSize), count(0)
    {
        table = new Node * [capacity];
        for (int i = 0; i < capacity; ++i) {
            table[i] = nullptr;
        }
    }

    /**
     * @brief Destroys the HashTable object.
     *
     * Clears all nodes in the table and deallocates the buckets array.
     */
    ~HashTable() {
        clear();
        delete[] table;
        table = nullptr;
    }

    // Disable copy constructor/assignment for simplicity.
    HashTable(const HashTable&) = delete;
    HashTable& operator=(const HashTable&) = delete;

    /**
     * @brief Inserts an item into the hash table.
     *
     * If an item with the same key already exists, it is updated with the new item.
     *
     * @param item The item to insert.
     */
    void insert(const T& item) {
        int key = getKey(item);
        int index = hashFunc(key);

        // Check if an item with this key already exists.
        Node* cur = table[index];
        while (cur) {
            if (getKey(cur->data) == key) {
                // Update existing data.
                cur->data = item;
                return;
            }
            cur = cur->next;
        }

        // Insert new node at the front of the bucket.
        Node* newNode = new Node(item);
        newNode->next = table[index];
        table[index] = newNode;
        ++count;
    }

    /**
     * @brief Removes an item by key from the hash table.
     *
     * Searches for an item with the given key and removes it if found.
     *
     * @param key The key of the item to remove.
     * @return True if the item was found and removed; false otherwise.
     */
    bool remove(int key) {
        int index = hashFunc(key);
        Node* cur = table[index];
        Node* prev = nullptr;

        while (cur) {
            if (getKey(cur->data) == key) {
                // Found the item; remove it.
                if (prev) {
                    prev->next = cur->next;
                }
                else {
                    table[index] = cur->next;
                }
                delete cur;
                --count;
                return true;
            }
            prev = cur;
            cur = cur->next;
        }
        return false; // Item not found.
    }

    /**
     * @brief Finds an item by key in the hash table.
     *
     * Searches for an item with the given key.
     *
     * @param key The key to search for.
     * @return A pointer to the item if found; nullptr otherwise.
     */
    T* find(int key) const {
        int index = hashFunc(key);
        Node* cur = table[index];
        while (cur) {
            if (getKey(cur->data) == key) {
                return &cur->data;
            }
            cur = cur->next;
        }
        return nullptr;
    }

    /**
     * @brief Clears the hash table.
     *
     * Removes all items from the table and resets the count.
     */
    void clear() {
        for (int i = 0; i < capacity; ++i) {
            Node* cur = table[i];
            while (cur) {
                Node* toDelete = cur;
                cur = cur->next;
                delete toDelete;
            }
            table[i] = nullptr;
        }
        count = 0;
    }

    /**
     * @brief Checks if the hash table is empty.
     *
     * @return True if the table is empty; false otherwise.
     */
    bool isEmpty() const {
        return (count == 0);
    }

    /**
     * @brief Returns the number of items in the hash table.
     *
     * @return The total number of items.
     */
    int size() const {
        return count;
    }

    /**
     * @brief Applies a function to each item in the hash table.
     *
     * Iterates over all items in the table and applies the given function.
     * If the function returns true, iteration is terminated early.
     *
     * @tparam Func The type of the function to apply.
     * @param fn The function to apply to each item.
     */
    template <typename Func>
    void forEach(Func fn) const {
        for (int i = 0; i < capacity; ++i) {
            Node* cur = table[i];
            while (cur) {
                if (fn(cur->data)) {
                    return;
                }
                cur = cur->next;
            }
        }
    }

    /**
     * @brief Retrieves the total number of items in the hash table.
     *
     * @return The total count of items.
     */
    int getCount() const {
        return size();
    }
};



#endif // HASHTABLE_H
