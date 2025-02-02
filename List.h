#ifndef LIST_H
#define LIST_H

#include <iostream>
#include <functional> // For std::function

/***************************************************************************
 * List.h
 *
 * Team:            Gearoid, Cedric
 * Group:           1
 * Student IDs:     S10241866, S10241549
 *
 * Features Highlight:
 *   - Template-based singly linked list implementation.
 *   - Supports deep copy via copy constructor and assignment operator.
 *   - Provides functions for adding, removing, clearing, and iterating over items.
 *
 ***************************************************************************/

 /**
  * @brief A template-based singly linked list.
  *
  * Provides basic operations such as add, remove, clear, and iteration.
  *
  * @tparam T The type of elements stored in the list.
  */
template <typename T>
class List {
private:
    /**
     * @brief Node structure representing an element in the list.
     */
    struct Node {
        T data;       ///< The data stored in this node.
        Node* next;   ///< Pointer to the next node in the list.

        /**
         * @brief Constructs a Node with the given data.
         *
         * @param d The data to store in the node.
         */
        Node(const T& d);
    };

    Node* head;  ///< Pointer to the head (first node) of the list.
    int   size;  ///< Tracks the number of elements in the list.

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
     * Creates a deep copy of the provided list.
     *
     * @param other The list to copy from.
     */
    List(const List<T>& other);

    /**
     * @brief Copy assignment operator.
     *
     * Clears the current list and copies all elements from the other list.
     *
     * @param other The list to assign from.
     * @return Reference to the current list.
     */
    List<T>& operator=(const List<T>& other);

    /**
     * @brief Destructor.
     *
     * Ensures proper memory cleanup by clearing the list.
     */
    ~List();

    // --------------------------
    // Public Member Functions
    // --------------------------

    /**
     * @brief Clears the list and releases all allocated memory.
     */
    void clear();

    /**
     * @brief Adds a new item to the end of the list.
     *
     * @param item The item to add.
     */
    void add(const T& item);

    /**
     * @brief Removes an item from the list.
     *
     * Searches for the first occurrence of the item and removes it.
     *
     * @param item The item to remove.
     * @return True if the item was found and removed; false otherwise.
     */
    bool remove(const T& item);

    /**
     * @brief Applies a function to each element in the list.
     *
     * Iterates over the list and applies the provided function to each element.
     * The iteration stops early if the function returns true.
     *
     * @param fn A callable that takes a const reference to an element and returns a bool.
     */
    void display(const std::function<bool(const T&)>& fn) const;

    /**
     * @brief Checks if the list is empty.
     *
     * @return True if the list is empty; false otherwise.
     */
    bool isEmpty() const;

    /**
     * @brief Retrieves the number of elements in the list.
     *
     * @return The size of the list.
     */
    int getSize() const;

    /**
     * @brief Retrieves the count of elements in the list.
     *
     * This function is synonymous with getSize().
     *
     * @return The number of elements in the list.
     */
    int getCount() const;
};

#endif // LIST_H
