#ifndef LIST_H
#define LIST_H

#include <iostream>
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

// A template-based singly linked list that supports basic operations
// like adding, removing, clearing, and iterating over elements.
template <typename T>
class List {
private:
    // Structure representing a node in the linked list.
    struct Node {
        T data;       // The actual data stored in the node
        Node* next;   // Pointer to the next node in the list

        // Constructor to initialize a node with data
        Node(const T& d);
    };

    Node* head;  // Pointer to the first node in the list
    int   size;  // Keeps track of the number of elements in the list

public:
    // --------------------------
    // Constructors and Destructor
    // --------------------------

    // Default constructor initializes an empty list.
    List();

    // Copy constructor creates a deep copy of another list.
    List(const List<T>& other);

    // Assignment operator clears the current list and copies another list.
    List<T>& operator=(const List<T>& other);

    // Destructor ensures proper memory cleanup.
    ~List();

    // --------------------------
    // Public Member Functions
    // --------------------------

    // Clears all elements in the list and deallocates memory.
    void clear();

    // Adds a new item to the end of the list.
    void add(const T& item);

    // Removes an item from the list. Returns true if successful.
    bool remove(const T& item);

    // Iterates over the list and applies a function to each element.
    // If the function returns true, the iteration stops early.
    void display(const std::function<bool(const T&)>& fn) const;

    // Checks if the list is empty.
    bool isEmpty() const;

    // Returns the number of elements in the list.
    int getSize() const;

    // Retrieves the total count of elements, same as getSize().
    int getCount() const;
};

#endif // LIST_H
