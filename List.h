#ifndef LIST_H
#define LIST_H

#include <iostream>
#include <functional> // For std::function

/**
 * @file List.h
 * @brief Template-based singly linked list that supports basic operations such as
 * adding, removing, clearing, and iterating over elements.
 *
 * Team:            Gearoid, Cedric
 * Group:           1
 * Student IDs:     S10241866, S10241549
 *
 * Features Highlight:
 *   - Represents a singly linked list for generic types.
 *   - Provides safe copying and assignment operations.
 *   - Includes functions to add, remove, clear, and iterate over elements.
 */

 /**
  * @brief Template-based singly linked list class.
  *
  * This class implements a simple singly linked list that supports basic operations.
  *
  * @tparam T The type of elements stored in the list.
  */
template <typename T>
class List {
private:
    /**
     * @brief Structure representing a node in the linked list.
     */
    struct Node {
        T data;       ///< The actual data stored in the node.
        Node* next;   ///< Pointer to the next node in the list.

        /**
         * @brief Constructor to initialize a node with data.
         *
         * @param d The data to be stored in the node.
         */
        Node(const T& d);
    };

    Node* head;  ///< Pointer to the first node in the list.
    int   size;  ///< Keeps track of the number of elements in the list.

public:
    // --------------------------
    // Constructors and Destructor
    // --------------------------

    /**
     * @brief Default constructor.
     *
     * Initializes an empty list.
     */
    List();

    /**
     * @brief Copy constructor.
     *
     * Creates a deep copy of another list.
     *
     * @param other The list to copy from.
     */
    List(const List<T>& other);

    /**
     * @brief Assignment operator.
     *
     * Clears the current list and copies the elements from another list.
     *
     * @param other The list to assign from.
     * @return A reference to this list after assignment.
     */
    List<T>& operator=(const List<T>& other);

    /**
     * @brief Destructor.
     *
     * Ensures proper memory cleanup by deallocating all nodes in the list.
     */
    ~List();

    // --------------------------
    // Public Member Functions
    // --------------------------

    /**
     * @brief Clears all elements in the list.
     *
     * Deallocates all nodes and resets the list size.
     */
    void clear();

    /**
     * @brief Adds a new item to the end of the list.
     *
     * @param item The item to be added.
     */
    void add(const T& item);

    /**
     * @brief Removes an item from the list.
     *
     * Searches for the item in the list and removes it if found.
     *
     * @param item The item to remove.
     * @return True if the item was successfully removed, false otherwise.
     */
    bool remove(const T& item);

    /**
     * @brief Iterates over the list and applies a function to each element.
     *
     * The iteration stops early if the function returns true.
     *
     * @param fn The function to apply to each element.
     */
    void display(const std::function<bool(const T&)>& fn) const;

    /**
     * @brief Checks if the list is empty.
     *
     * @return True if the list has no elements, false otherwise.
     */
    bool isEmpty() const;

    /**
     * @brief Returns the number of elements in the list.
     *
     * @return The size of the list.
     */
    int getSize() const;

    /**
     * @brief Retrieves the total count of elements in the list.
     *
     * This is equivalent to getSize().
     *
     * @return The total number of elements.
     */
    int getCount() const;
};

#endif // LIST_H
