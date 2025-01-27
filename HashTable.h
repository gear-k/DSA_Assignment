// HashTable.h
#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <iostream>  // For optional debug prints
#include <cstring>   // For strcmp, etc.

// Forward declaration: a function template to get the integer key from type T.
template<typename T>
int getKey(const T& item);

// ---------------------------------------------------------------------
// A simple separate-chaining HashTable storing type T.
// Key collisions are handled via linked-list chaining.
//
// This example does NOT do dynamic resizing; choose an appropriate
// bucket count (capacity) for your data size.
//
// We assume T has a unique integer key accessible via getKey(T).
// ---------------------------------------------------------------------
template <typename T>
class HashTable {
private:
    struct Node {
        T     data;
        Node* next;
        Node(const T& d) : data(d), next(nullptr) {}
    };

    Node** table;   // An array of pointers to Nodes (the buckets)
    int    capacity; // Number of buckets
    int    count;    // Number of total items stored

    // A simple hash function: key mod capacity (kept non-negative)
    int hashFunc(int key) const {
        return (key % capacity + capacity) % capacity;
    }

public:
    // Constructor
    HashTable(int tableSize = 2000)
        : capacity(tableSize), count(0)
    {
        table = new Node * [capacity];
        for (int i = 0; i < capacity; ++i) {
            table[i] = nullptr;
        }
    }

    // Destructor
    ~HashTable() {
        clear();
        delete[] table;
        table = nullptr;
    }

    // Disable copy constructor/assignment for simplicity
    HashTable(const HashTable&) = delete;
    HashTable& operator=(const HashTable&) = delete;

    // Insert (or update if key already exists)
    void insert(const T& item) {
        int key = getKey(item);
        int index = hashFunc(key);

        // Check if item with this key already exists
        Node* cur = table[index];
        while (cur) {
            if (getKey(cur->data) == key) {
                // Update existing data
                cur->data = item;
                return;
            }
            cur = cur->next;
        }

        // Otherwise, insert new node at the front
        Node* newNode = new Node(item);
        newNode->next = table[index];
        table[index] = newNode;
        ++count;
    }

    // Remove by integer key; returns true if found & removed
    bool remove(int key) {
        int index = hashFunc(key);
        Node* cur = table[index];
        Node* prev = nullptr;

        while (cur) {
            if (getKey(cur->data) == key) {
                // Found it; remove
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
        return false; // Not found
    }

    // Find by integer key; returns pointer to item or nullptr
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

    // Clear the entire table
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

    // Check if empty
    bool isEmpty() const {
        return (count == 0);
    }

    // Return number of items
    int size() const {
        return count;
    }

    // forEach to iterate all items. 
    // If `fn` returns true, we break early.
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

    int getCount() const {
        return size();
    }
};

#endif // HASHTABLE_H
