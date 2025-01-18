#ifndef LIST_H
#define LIST_H

#include <iostream> // Allowed for printing; remove if strictly not permitted.

template <typename T>
class List {
private:
    struct Node {
        T     data;       // Stores the data of the node
        Node* next;       // Points to the next node in the list
        Node(const T& d) : data(d), next(nullptr) {} // Constructor for a node
    };

    Node* head;           // Head pointer for the list
    int   size;           // Tracks the size of the list

public:
    // Default Constructor
    List() : head(nullptr), size(0) {}

    // Copy Constructor
    List(const List<T>& other) : head(nullptr), size(0) {
        Node* current = other.head;
        while (current) {
            add(current->data);
            current = current->next;
        }
    }

    // Copy Assignment Operator
    List<T>& operator=(const List<T>& other) {
        if (this != &other) { // Prevent self-assignment
            // Clear existing list
            Node* cur = head;
            while (cur) {
                Node* tmp = cur;
                cur = cur->next;
                delete tmp;
            }
            head = nullptr;
            size = 0;

            // Copy from other
            Node* current = other.head;
            while (current) {
                add(current->data);
                current = current->next;
            }
        }
        return *this;
    }

    // Destructor: Ensures proper memory cleanup
    ~List() {
        Node* cur = head;
        while (cur) {
            Node* tmp = cur;
            cur = cur->next;
            delete tmp;      // Safely delete the node
            tmp = nullptr;   // Avoid dangling pointers
        }
        head = nullptr;       // Reset the head pointer
    }

    // Add a new item to the end of the list
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

    // Remove an item from the list
    bool remove(const T& item) {
        if (!head) return false; // List is empty
        if (head->data == item) { // If the head contains the item
            Node* toDel = head;
            head = head->next;
            delete toDel;
            --size;
            return true;
        }

        Node* cur = head;
        while (cur->next && !(cur->next->data == item)) {
            cur = cur->next; // Traverse to find the item
        }
        if (cur->next) { // If the item is found
            Node* toDel = cur->next;
            cur->next = toDel->next;
            delete toDel;
            --size;
            return true;
        }
        return false; // Item not found
    }

    // Apply a function to each node; break early if `fn` returns true
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

    // Check if the list is empty
    bool isEmpty() const {
        return (head == nullptr);
    }

    // Get the size of the list
    int getSize() const {
        return size;
    }
};

#endif // LIST_H
