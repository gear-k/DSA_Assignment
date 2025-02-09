#include "HashTable.h"
#include "Actor.h"
#include "Movie.h"
#include <functional>

/**
 * @brief Constructs a new Node for the HashTable.
 *
 * Initializes the node with the provided data and sets the next pointer to nullptr.
 *
 * @tparam T The type of data stored in the node.
 * @param d The data to be stored in the node.
 */
template <typename T>
HashTable<T>::Node::Node(const T& d)
    : data(d), next(nullptr) {
}

/**
 * @brief Computes the hash index for a given key.
 *
 * Uses a modulo operation to compute the index, ensuring that the result
 * is within the valid range [0, capacity-1].
 *
 * @tparam T The type of data stored in the hash table.
 * @param key The key to be hashed.
 * @return The computed hash index.
 */
template <typename T>
int HashTable<T>::hashFunc(int key) const {
    return (key % capacity + capacity) % capacity;
}

/**
 * @brief Constructs a new HashTable.
 *
 * Initializes the hash table with the given table size and allocates an array of
 * buckets (pointers to Node). Each bucket is initialized to nullptr.
 *
 * @tparam T The type of data stored in the hash table.
 * @param tableSize The size (capacity) of the hash table.
 */
template <typename T>
HashTable<T>::HashTable(int tableSize)
    : capacity(tableSize), count(0)
{
    table = new Node * [capacity];
    for (int i = 0; i < capacity; ++i) {
        table[i] = nullptr;
    }
}

/**
 * @brief Destroys the HashTable.
 *
 * Clears the hash table, deallocating all nodes, and then deallocates the
 * underlying bucket array.
 *
 * @tparam T The type of data stored in the hash table.
 */
template <typename T>
HashTable<T>::~HashTable() {
    clear();
    delete[] table;
    table = nullptr;
}

/**
 * @brief Inserts an item into the hash table.
 *
 * If an item with the same key already exists, this function updates that item.
 * Otherwise, it inserts a new node at the front of the bucket corresponding to the key.
 *
 * @tparam T The type of data stored in the hash table.
 * @param item The item to be inserted.
 */
template <typename T>
void HashTable<T>::insert(const T& item) {
    int key = getKey(item);
    int index = hashFunc(key);

    // Check if an item with the same key already exists and update it.
    Node* cur = table[index];
    while (cur) {
        if (getKey(cur->data) == key) {
            cur->data = item;
            return;
        }
        cur = cur->next;
    }

    // Insert new node at the beginning of the bucket.
    Node* newNode = new Node(item);
    newNode->next = table[index];
    table[index] = newNode;
    ++count;
}

/**
 * @brief Removes an item with the specified key from the hash table.
 *
 * Searches for the item by key and removes it if found.
 *
 * @tparam T The type of data stored in the hash table.
 * @param key The key of the item to remove.
 * @return true if the item was found and removed; false otherwise.
 */
template <typename T>
bool HashTable<T>::remove(int key) {
    int index = hashFunc(key);
    Node* cur = table[index];
    Node* prev = nullptr;

    while (cur) {
        if (getKey(cur->data) == key) {
            // Remove the found node.
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
 * Searches for an item with the specified key and returns a pointer to the item
 * if found, or nullptr if the item does not exist.
 *
 * @tparam T The type of data stored in the hash table.
 * @param key The key of the item to find.
 * @return A pointer to the found item, or nullptr if not found.
 */
template <typename T>
T* HashTable<T>::find(int key) const {
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
 * @brief Clears all items from the hash table.
 *
 * Deallocates all nodes in every bucket and resets the item count to zero.
 *
 * @tparam T The type of data stored in the hash table.
 */
template <typename T>
void HashTable<T>::clear() {
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
 * @tparam T The type of data stored in the hash table.
 * @return true if the hash table contains no items; false otherwise.
 */
template <typename T>
bool HashTable<T>::isEmpty() const {
    return (count == 0);
}

/**
 * @brief Returns the number of items in the hash table.
 *
 * @tparam T The type of data stored in the hash table.
 * @return The number of items currently stored.
 */
template <typename T>
int HashTable<T>::size() const {
    return count;
}

/**
 * @brief Applies a function to each item in the hash table.
 *
 * Iterates through all items in the hash table and calls the provided function
 * on each item. The iteration stops if the function returns true.
 *
 * @tparam T The type of data stored in the hash table.
 * @param fn A function that takes a constant reference to an item and returns a bool.
 */
template <typename T>
void HashTable<T>::forEach(const std::function<bool(const T&)>& fn) const {
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
 * @tparam T The type of data stored in the hash table.
 * @return The total item count.
 */
template <typename T>
int HashTable<T>::getCount() const {
    return size();
}



// ***** Template Specializations for getKey *****

/**
 * @brief Specialization of getKey for int.
 *
 * Returns the integer item itself as the key.
 *
 * @param item The integer item.
 * @return The key corresponding to the item.
 */
template <>
int getKey<int>(const int& item) {
    return item;
}

/**
 * @brief Specialization of getKey for Actor.
 *
 * Returns the actor's unique ID.
 *
 * @param item The Actor item.
 * @return The actor's ID.
 */
template <>
int getKey<Actor>(const Actor& item) {
    return item.getId();
}

/**
 * @brief Specialization of getKey for Movie.
 *
 * Returns the movie's unique ID.
 *
 * @param item The Movie item.
 * @return The movie's ID.
 */
template <>
int getKey<Movie>(const Movie& item) {
    return item.getId();
}

// ***** Explicit Template Instantiations *****
template class HashTable<int>;
template class HashTable<Actor>;
template class HashTable<Movie>;