#include "HashTable.h"
#include "Actor.h"
#include "Movie.h"

// ***** Template Definitions *****

// Node constructor initializes a node with the given data and sets next to nullptr.
template <typename T>
HashTable<T>::Node::Node(const T& d)
    : data(d), next(nullptr) {
}

// Computes the hash index for a given key using modulo operation.
template <typename T>
int HashTable<T>::hashFunc(int key) const {
    return (key % capacity + capacity) % capacity;
}

// Constructor initializes the hash table with the given table size.
template <typename T>
HashTable<T>::HashTable(int tableSize)
    : capacity(tableSize), count(0)
{
    table = new Node * [capacity];
    for (int i = 0; i < capacity; ++i) {
        table[i] = nullptr;
    }
}

// Destructor clears the hash table and deallocates memory.
template <typename T>
HashTable<T>::~HashTable() {
    clear();
    delete[] table;
    table = nullptr;
}

// Inserts an item into the hash table.
template <typename T>
void HashTable<T>::insert(const T& item) {
    int key = getKey(item);
    int index = hashFunc(key);

    // Check if an item with this key already exists and update it.
    Node* cur = table[index];
    while (cur) {
        if (getKey(cur->data) == key) {
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

// Removes an item by key from the hash table.
template <typename T>
bool HashTable<T>::remove(int key) {
    int index = hashFunc(key);
    Node* cur = table[index];
    Node* prev = nullptr;

    while (cur) {
        if (getKey(cur->data) == key) {
            // If the item is found, remove it.
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

// Finds an item by key in the hash table.
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

// Clears all items from the hash table.
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

// Checks if the hash table is empty.
template <typename T>
bool HashTable<T>::isEmpty() const {
    return (count == 0);
}

// Returns the number of items currently in the hash table.
template <typename T>
int HashTable<T>::size() const {
    return count;
}

// Applies a function to each item in the hash table.
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

// Retrieves the total number of items in the hash table.
template <typename T>
int HashTable<T>::getCount() const {
    return size();
}

// Explicit template instantiations.
template class HashTable<int>;
template class HashTable<Actor>;
template class HashTable<Movie>;

// Specialization for int key retrieval.
template <>
int getKey<int>(const int& item) {
    return item;
}

// Specialization for Actor key retrieval (returns actor ID).
#include "Actor.h"
template <>
int getKey<Actor>(const Actor& item) {
    return item.getId();
}

// Specialization for Movie key retrieval (returns movie ID).
#include "Movie.h"
template <>
int getKey<Movie>(const Movie& item) {
    return item.getId();
}