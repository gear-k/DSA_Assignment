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
    : data(d), next(nullptr)
{
}

/**
 * @brief Computes the hash index for a given key.
 *
 * Uses a modulo operation to compute the index ensuring it is within the valid range.
 *
 * @tparam T The type of data stored in the hash table.
 * @param key The key to be hashed.
 * @return int The computed hash index.
 */
template <typename T>
int HashTable<T>::hashFunc(int key) const {
    return (key % capacity + capacity) % capacity;
}

/**
 * @brief Constructs a HashTable with a specified number of buckets and load factor.
 *
 * Initializes the hash table with the given initial capacity and maximum load factor.
 *
 * @tparam T The type of data stored in the hash table.
 * @param tableSize The initial number of buckets in the hash table.
 * @param maxLoadFactor The maximum load factor before resizing occurs.
 */
template <typename T>
HashTable<T>::HashTable(int tableSize, double maxLoadFactor)
    : capacity(tableSize), count(0), maxLoadFactor(maxLoadFactor)
{
    table = new Node * [capacity];
    for (int i = 0; i < capacity; ++i) {
        table[i] = nullptr;
    }
}

/**
 * @brief Destructor for the HashTable.
 *
 * Clears and deallocates all nodes and the underlying bucket array.
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
 * @brief Resizes the hash table to a new capacity.
 *
 * Rehashes all existing items into a new bucket array with the new capacity.
 *
 * @tparam T The type of data stored in the hash table.
 * @param newCapacity The new number of buckets for the hash table.
 */
template <typename T>
void HashTable<T>::rehash(int newCapacity) {
    Node** newTable = new Node * [newCapacity];
    for (int i = 0; i < newCapacity; ++i) {
        newTable[i] = nullptr;
    }

    // Rehash nodes from the old table into the new table.
    for (int i = 0; i < capacity; ++i) {
        Node* cur = table[i];
        while (cur) {
            Node* nextNode = cur->next;
            int newIndex = (getKey(cur->data) % newCapacity + newCapacity) % newCapacity;
            cur->next = newTable[newIndex];
            newTable[newIndex] = cur;
            cur = nextNode;
        }
    }

    delete[] table;
    table = newTable;
    capacity = newCapacity;
}

/**
 * @brief Inserts an item into the hash table.
 *
 * If an item with the same key exists, updates the item; otherwise, inserts a new item.
 * Resizes the table if the load factor is exceeded.
 *
 * @tparam T The type of data stored in the hash table.
 * @param item The item to insert.
 */
template <typename T>
void HashTable<T>::insert(const T& item) {
    // Check if the load factor threshold is exceeded; if so, resize.
    if (static_cast<double>(count + 1) / capacity > maxLoadFactor) {
        rehash(capacity * 2);  // Resize: double the capacity.
    }

    int key = getKey(item);
    int index = hashFunc(key);

    // Check if an item with the same key exists; if so, update it.
    Node* cur = table[index];
    while (cur) {
        if (getKey(cur->data) == key) {
            cur->data = item;
            return;
        }
        cur = cur->next;
    }

    // Insert a new node at the beginning of the bucket.
    Node* newNode = new Node(item);
    newNode->next = table[index];
    table[index] = newNode;
    ++count;
}

/**
 * @brief Removes an item from the hash table based on its key.
 *
 * Searches for the item with the given key and removes it if found.
 *
 * @tparam T The type of data stored in the hash table.
 * @param key The key of the item to remove.
 * @return true If the item was successfully removed.
 * @return false If the item with the given key was not found.
 */
template <typename T>
bool HashTable<T>::remove(int key) {
    int index = hashFunc(key);
    Node* cur = table[index];
    Node* prev = nullptr;

    while (cur) {
        if (getKey(cur->data) == key) {
            // Remove the node.
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
    return false;
}

/**
 * @brief Finds an item in the hash table by its key.
 *
 * Searches for an item with the specified key and returns a pointer to it if found.
 *
 * @tparam T The type of data stored in the hash table.
 * @param key The key of the item to find.
 * @return T* Pointer to the found item, or nullptr if not found.
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
 * @brief Checks whether the hash table is empty.
 *
 * @tparam T The type of data stored in the hash table.
 * @return true If the hash table contains no items.
 * @return false Otherwise.
 */
template <typename T>
bool HashTable<T>::isEmpty() const {
    return (count == 0);
}

/**
 * @brief Returns the number of stored items in the hash table.
 *
 * @tparam T The type of data stored in the hash table.
 * @return int The number of items.
 */
template <typename T>
int HashTable<T>::size() const {
    return count;
}

/**
 * @brief Iterates over all items in the hash table and applies a given function.
 *
 * The provided function is applied to each item; if the function returns true,
 * the iteration stops early.
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
 * @brief Retrieves the total number of items stored in the hash table.
 *
 * @tparam T The type of data stored in the hash table.
 * @return int The total count of items.
 */
template <typename T>
int HashTable<T>::getCount() const {
    return size();
}

// ***** Template Specializations for getKey *****

/**
 * @brief Specialization of getKey for int.
 *
 * Returns the integer itself as the key.
 *
 * @param item The integer item.
 * @return int The key corresponding to the item.
 */
template <>
int getKey<int>(const int& item) {
    return item;
}

/**
 * @brief Specialization of getKey for Actor.
 *
 * Returns the unique ID of the Actor.
 *
 * @param item The Actor item.
 * @return int The Actor's unique ID.
 */
template <>
int getKey<Actor>(const Actor& item) {
    return item.getId();
}

/**
 * @brief Specialization of getKey for Movie.
 *
 * Returns the unique ID of the Movie.
 *
 * @param item The Movie item.
 * @return int The Movie's unique ID.
 */
template <>
int getKey<Movie>(const Movie& item) {
    return item.getId();
}

// ***** Explicit Template Instantiations *****
template class HashTable<int>;
template class HashTable<Actor>;
template class HashTable<Movie>;
