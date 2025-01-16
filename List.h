#ifndef LIST_H
#define LIST_H

#include <iostream> // Allowed for printing; remove if strictly not permitted.

template <typename T>
class List {
private:
    struct Node {
        T     data;
        Node* next;
        Node(const T& d) : data(d), next(nullptr) {}
    };

    Node* head;
    int   size;

public:
    List() : head(nullptr), size(0) {}
    ~List() {
        Node* cur = head;
        while (cur) {
            Node* tmp = cur;
            cur = cur->next;
            delete tmp;
        }
    }

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

        Node* cur = head;
        while (cur->next && !(cur->next->data == item)) {
            cur = cur->next;
        }
        if (cur->next) {
            Node* toDel = cur->next;
            cur->next = toDel->next;
            delete toDel;
            --size;
            return true;
        }
        return false;
    }

    // Apply a function to each node; if fn(...) returns true, we break early
    template <typename Func>
    void display(Func fn) const {
        Node* cur = head;
        while (cur) {
            if (fn(cur->data)) {
                break;
            }
            cur = cur->next;
        }
    }

    bool isEmpty() const {
        return (head == nullptr);
    }

    int getSize() const {
        return size;
    }
};

#endif // LIST_H
