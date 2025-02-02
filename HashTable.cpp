#include "HashTable.h"
#include "Actor.h"
#include "Movie.h"

// ***** Template Definitions *****

// Node constructor.
template <typename T>
HashTable<T>::Node::Node(const T& d)
    : data(d), next(nullptr) {}

// Computes the hash index.
template <typename T>
int HashTable<T>::hashFunc(int key) const {
    return (key % capacity + capacity) % capacity;
}

// Constructor.
template <typename T>
HashTable<T>::HashTable(int tableSize)
    : capacity(tableSize), count(0)
{
    table = new Node * [capacity];
    for (int i = 0; i < capacity; ++i) {
        table[i] = nullptr;
    }
}

// Destructor.
template <typename T>
HashTable<T>::~HashTable() {
    clear();
    delete[] table;
    table = nullptr;
}

// Inserts an item.
template <typename T>
void HashTable<T>::insert(const T& item) {
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

// Removes an item by key.
template <typename T>
bool HashTable<T>::remove(int key) {
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

// Finds an item by key.
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

// Clears the hash table.
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

// Returns the number of items.
template <typename T>
int HashTable<T>::size() const {
    return count;
}

// Applies a function to each item.
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

// Retrieves the total number of items.
template <typename T>
int HashTable<T>::getCount() const {
    return size();
}

// ***** Explicit Template Instantiation *****
// Add explicit instantiation for each type you plan to use.
template class HashTable<int>;
template class HashTable<Actor>;
template class HashTable<Movie>;


// Specialization for int:
template <>
int getKey<int>(const int& item) {
    return item;
}

#include "Actor.h"
template <>
int getKey<Actor>(const Actor& item) {
    return item.getId();
}

#include "Movie.h"
template <>
int getKey<Movie>(const Movie& item) {
    return item.getId();
}