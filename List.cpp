#include "List.h"
#include "Actor.h"
#include "Movie.h"

// ***** Template Definitions *****

// Node constructor initializes a node with the given data and sets next to nullptr.
template <typename T>
List<T>::Node::Node(const T& d)
    : data(d), next(nullptr) {
}

// Default constructor initializes an empty list.
template <typename T>
List<T>::List()
    : head(nullptr), size(0) {
}

// Copy constructor creates a deep copy of another list.
template <typename T>
List<T>::List(const List<T>& other)
    : head(nullptr), size(0) {
    Node* current = other.head;
    while (current) {
        add(current->data);
        current = current->next;
    }
}

// Copy assignment operator clears the current list and copies from another.
template <typename T>
List<T>& List<T>::operator=(const List<T>& other) {
    if (this != &other) { // Prevent self-assignment
        clear();
        Node* current = other.head;
        while (current) {
            add(current->data);
            current = current->next;
        }
    }
    return *this;
}

// Destructor clears the list.
template <typename T>
List<T>::~List() {
    clear();
}

// Clears all elements in the list and deallocates memory.
template <typename T>
void List<T>::clear() {
    Node* cur = head;
    while (cur) {
        Node* tmp = cur;
        cur = cur->next;
        delete tmp;      // Safely delete the node.
    }
    head = nullptr;       // Reset the head pointer.
    size = 0;             // Reset the size.
}

// Adds a new item to the end of the list.
template <typename T>
void List<T>::add(const T& item) {
    Node* newNode = new Node(item);
    if (!head) {          // If the list is empty.
        head = newNode;
    }
    else {
        Node* temp = head;
        while (temp->next) { // Traverse to the end of the list.
            temp = temp->next;
        }
        temp->next = newNode; // Append the new node.
    }
    ++size;               // Increase the size.
}

// Removes an item from the list. Returns true if successful.
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

// Iterates over the list and applies a function to each element.
// The function can stop early by returning true.
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

// Checks if the list is empty.
template <typename T>
bool List<T>::isEmpty() const {
    return (head == nullptr);
}

// Returns the number of elements in the list.
template <typename T>
int List<T>::getSize() const {
    return size;
}

// Returns the total count of elements, same as getSize().
template <typename T>
int List<T>::getCount() const {
    return size;
}

// ***** Explicit Template Instantiation *****
// Instantiate for each type that you plan to use.
template class List<int>;
template class List<Actor>;
template class List<Movie>;
