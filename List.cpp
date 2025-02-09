#include "List.h"
#include "Actor.h"
#include "Movie.h"
#include <functional>

/**
 * @brief Node constructor for the List.
 *
 * Constructs a new node with the provided data and sets the next pointer to nullptr.
 *
 * @tparam T The type of data stored in the node.
 * @param d The data to be stored in the node.
 */
template <typename T>
List<T>::Node::Node(const T& d)
    : data(d), next(nullptr) {
}

/**
 * @brief Default constructor for List.
 *
 * Initializes an empty list by setting the head pointer to nullptr and the size to 0.
 *
 * @tparam T The type of data stored in the list.
 */
template <typename T>
List<T>::List()
    : head(nullptr), size(0) {
}

/**
 * @brief Copy constructor for List.
 *
 * Creates a deep copy of the provided list by iterating through its nodes and adding
 * each element to the new list.
 *
 * @tparam T The type of data stored in the list.
 * @param other The list from which to create a deep copy.
 */
template <typename T>
List<T>::List(const List<T>& other)
    : head(nullptr), size(0) {
    Node* current = other.head;
    while (current) {
        add(current->data);
        current = current->next;
    }
}

/**
 * @brief Copy assignment operator for List.
 *
 * Clears the current list and then performs a deep copy of the provided list.
 * Prevents self-assignment.
 *
 * @tparam T The type of data stored in the list.
 * @param other The list from which to copy.
 * @return A reference to the current list.
 */
template <typename T>
List<T>& List<T>::operator=(const List<T>& other) {
    if (this != &other) { // Prevent self-assignment.
        clear();
        Node* current = other.head;
        while (current) {
            add(current->data);
            current = current->next;
        }
    }
    return *this;
}

/**
 * @brief Destructor for List.
 *
 * Clears the list and deallocates memory used by its nodes.
 *
 * @tparam T The type of data stored in the list.
 */
template <typename T>
List<T>::~List() {
    clear();
}

/**
 * @brief Clears all elements in the list.
 *
 * Iterates over the list, deallocates memory for each node, and resets the head pointer
 * and size to represent an empty list.
 *
 * @tparam T The type of data stored in the list.
 */
template <typename T>
void List<T>::clear() {
    Node* cur = head;
    while (cur) {
        Node* tmp = cur;
        cur = cur->next;
        delete tmp; // Safely delete the node.
    }
    head = nullptr; // Reset the head pointer.
    size = 0;       // Reset the size.
}

/**
 * @brief Adds a new item to the end of the list.
 *
 * Creates a new node with the provided item and appends it to the end of the list.
 *
 * @tparam T The type of data stored in the list.
 * @param item The item to be added.
 */
template <typename T>
void List<T>::add(const T& item) {
    Node* newNode = new Node(item);
    if (!head) { // If the list is empty.
        head = newNode;
    }
    else {
        Node* temp = head;
        while (temp->next) { // Traverse to the end of the list.
            temp = temp->next;
        }
        temp->next = newNode; // Append the new node.
    }
    ++size; // Increase the size.
}

/**
 * @brief Removes an item from the list.
 *
 * Searches for the first occurrence of the specified item and removes it from the list.
 *
 * @tparam T The type of data stored in the list.
 * @param item The item to be removed.
 * @return true if the item was found and removed; false otherwise.
 */
template <typename T>
bool List<T>::remove(const T& item) {
    if (!head) return false; // List is empty.

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
 * @brief Iterates over the list and applies a function to each element.
 *
 * Traverses the list and calls the provided function on each element. If the function
 * returns true for an element, the iteration stops early.
 *
 * @tparam T The type of data stored in the list.
 * @param fn A function that takes a const reference to an element and returns a bool.
 */
template <typename T>
void List<T>::display(const std::function<bool(const T&)>& fn) const {
    Node* cur = head;
    while (cur) {
        if (fn(cur->data)) {
            break; // Exit early if the function returns true.
        }
        cur = cur->next;
    }
}

/**
 * @brief Checks if the list is empty.
 *
 * @tparam T The type of data stored in the list.
 * @return true if the list is empty; false otherwise.
 */
template <typename T>
bool List<T>::isEmpty() const {
    return (head == nullptr);
}

/**
 * @brief Returns the number of elements in the list.
 *
 * @tparam T The type of data stored in the list.
 * @return The number of elements in the list.
 */
template <typename T>
int List<T>::getSize() const {
    return size;
}

/**
 * @brief Returns the total count of elements in the list.
 *
 * This function is equivalent to getSize().
 *
 * @tparam T The type of data stored in the list.
 * @return The number of elements in the list.
 */
template <typename T>
int List<T>::getCount() const {
    return size;
}

// ***** Explicit Template Instantiation *****
// Instantiate the List template for each type that will be used.
template class List<int>;
template class List<Actor>;
template class List<Movie>;
