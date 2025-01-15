#ifndef LIST_H
#define LIST_H

#include <iostream>
#include <functional>

template <typename T>
class List {
private:
    struct Node {
        T data;
        Node* next;
        Node(const T& value) : data(value), next(nullptr) {}
    };
    Node* head;
    int size;

public:
    // Constructors / destructor
    List() : head(nullptr), size(0) {}

    ~List() {
        Node* temp;
        while (head) {
            temp = head;
            head = head->next;
            delete temp;
        }
    }

    // Core functionality
    void add(const T& item) {
        Node* newNode = new Node(item);
        if (!head) {
            head = newNode;
        }
        else {
            Node* temp = head;
            while (temp->next) {
                temp = temp->next;
            }
            temp->next = newNode;
        }
        ++size;
    }

    bool remove(const T& item) {
        if (!head) return false;
        if (head->data == item) {
            Node* toDel = head;
            head = head->next;
            delete toDel;
            --size;
            return true;
        }
        Node* current = head;
        while (current->next && !(current->next->data == item)) {
            current = current->next;
        }
        if (current->next) {
            Node* toDel = current->next;
            current->next = toDel->next;
            delete toDel;
            --size;
            return true;
        }
        return false;
    }

    // Display entire list using T's operator<<
    void display() const {
        Node* temp = head;
        while (temp) {
            std::cout << temp->data << std::endl;
            temp = temp->next;
        }
    }

    // Overload for callback iteration
    template <typename Func>
    void display(Func func) const {
        Node* current = head;
        while (current) {
            if (func(current->data)) {
                break;
            }
            current = current->next;
        }
    }

    // Utility
    bool isEmpty() const { return (head == nullptr); }
    int getSize() const { return size; }
};

#endif // LIST_H