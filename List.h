#ifndef LIST_H
#define LIST_H

#include <iostream> // Allowed for printing; remove if strictly not permitted.

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
        Node(const T& d) : data(d), next(nullptr) {}
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
    List() : head(nullptr), size(0) {}

    /**
     * @brief Copy constructor.
     *
     * Creates a deep copy of the provided list.
     *
     * @param other The list to copy from.
     */
    List(const List<T>& other) : head(nullptr), size(0) {
        Node* current = other.head;
        while (current) {
            add(current->data);
            current = current->next;
        }
    }

    /**
     * @brief Copy assignment operator.
     *
     * Clears the current list and copies all elements from the other list.
     *
     * @param other The list to assign from.
     * @return Reference to the current list.
     */
    List<T>& operator=(const List<T>& other) {
        if (this != &other) { // Prevent self-assignment
            clear(); // Clear existing list
            Node* current = other.head;
            while (current) {
                add(current->data);
                current = current->next;
            }
        }
        return *this;
    }

    /**
     * @brief Destructor.
     *
     * Ensures proper memory cleanup by clearing the list.
     */
    ~List() {
        clear();
    }

    // --------------------------
    // Public Member Functions
    // --------------------------

    /**
     * @brief Clears the list and releases all allocated memory.
     */
    void clear() {
        Node* cur = head;
        while (cur) {
            Node* tmp = cur;
            cur = cur->next;
            delete tmp;      // Safely delete the node
            tmp = nullptr;   // Avoid dangling pointers
        }
        head = nullptr;       // Reset the head pointer
        size = 0;             // Reset the size
    }

    /**
     * @brief Adds a new item to the end of the list.
     *
     * @param item The item to add.
     */
    void add(const T& item) {
        Node* newNode = new Node(item);
        if (!head) {          // If the list is empty
            head = newNode;
        }
        else {
            Node* temp = head;
            while (temp->next) { // Traverse to the end of the list
                temp = temp->next;
            }
            temp->next = newNode; // Append the new node
        }
        ++size;               // Increase the size
    }

    /**
     * @brief Removes an item from the list.
     *
     * Searches for the first occurrence of the item and removes it.
     *
     * @param item The item to remove.
     * @return True if the item was found and removed; false otherwise.
     */
    bool remove(const T& item) {
        if (!head) return false; // List is empty

        // Check if the head node contains the item.
        if (head->data == item) {
            Node* toDel = head;
            head = head->next;
            delete toDel;
            --size;
            return true;
        }

        // Traverse the list to find the item.
        Node* cur = head;
        while (cur->next && !(cur->next->data == item)) {
            cur = cur->next;
        }
        if (cur->next) { // Item found.
            Node* toDel = cur->next;
            cur->next = toDel->next;
            delete toDel;
            --size;
            return true;
        }
        return false; // Item not found.
    }

    /**
     * @brief Applies a function to each element in the list.
     *
     * Iterates over the list and applies the provided function to each element.
     * The iteration stops early if the function returns true.
     *
     * @tparam Func The type of the function to apply.
     * @param fn The function to apply to each element.
     */
    template <typename Func>
    void display(Func fn) const {
        Node* cur = head;
        while (cur) {
            if (fn(cur->data)) {
                break; // Exit early if the function returns true
            }
            cur = cur->next;
        }
    }

    /**
     * @brief Checks if the list is empty.
     *
     * @return True if the list is empty; false otherwise.
     */
    bool isEmpty() const {
        return (head == nullptr);
    }

    /**
     * @brief Retrieves the number of elements in the list.
     *
     * @return The size of the list.
     */
    int getSize() const {
        return size;
    }

    /**
     * @brief Retrieves the count of elements in the list.
     *
     * This function is synonymous with getSize().
     *
     * @return The number of elements in the list.
     */
    int getCount() const {
        return size;
    }
};

#endif // LIST_H
