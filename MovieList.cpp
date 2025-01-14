#include "MovieList.h"
#include <iostream>

MovieList::MovieList() : head(nullptr), size(0) {}

MovieList::~MovieList() {
    while (head) {
        Node* temp = head;
        head = head->next;
        delete temp;
    }
}

bool MovieList::add(const Movie& movie) {
    Node* newNode = new Node{ movie, nullptr };
    if (!head) {
        head = newNode;
    }
    else {
        Node* temp = head;
        while (temp->next) temp = temp->next;
        temp->next = newNode;
    }
    size++;
    return true;
}

bool MovieList::remove(int index) {
    if (index < 1 || index > size) return false;
    Node* temp = head;

    if (index == 1) {
        head = head->next;
    }
    else {
        Node* prev = nullptr;
        for (int i = 1; i < index; ++i) {
            prev = temp;
            temp = temp->next;
        }
        prev->next = temp->next;
    }

    delete temp;
    size--;
    return true;
}

Movie* MovieList::findById(int id) {
    Node* temp = head;
    while (temp) {
        if (temp->movie.getId() == id) return &temp->movie;
        temp = temp->next;
    }
    return nullptr;
}

Movie* MovieList::get(int index) {
    if (index < 1 || index > size) return nullptr;
    Node* temp = head;
    for (int i = 1; i < index; ++i) {
        temp = temp->next;
    }
    return &temp->movie;
}

const Movie* MovieList::get(int index) const {
    if (index < 1 || index > size) return nullptr;
    Node* temp = head;
    for (int i = 1; i < index; ++i) {
        temp = temp->next;
    }
    return &temp->movie;
}

int MovieList::getLength() const {
    return size;
}

bool MovieList::isEmpty() const {
    return size == 0;
}

void MovieList::displayAll() const {
    Node* temp = head;
    while (temp) {
        temp->movie.displayDetails();
        temp = temp->next;
    }
}

MovieList::Node* MovieList::mergeSortedLists(Node* left, Node* right) {
    if (!left) return right;
    if (!right) return left;

    if (left->movie.getTitle() < right->movie.getTitle()) {
        left->next = mergeSortedLists(left->next, right);
        return left;
    }
    else {
        right->next = mergeSortedLists(left, right->next);
        return right;
    }
}

MovieList::Node* MovieList::mergeSort(Node* node) {
    if (!node || !node->next) return node;

    Node* slow = node;
    Node* fast = node->next;
    while (fast && fast->next) {
        slow = slow->next;
        fast = fast->next->next;
    }

    Node* mid = slow->next;
    slow->next = nullptr;

    Node* left = mergeSort(node);
    Node* right = mergeSort(mid);

    return mergeSortedLists(left, right);
}

void MovieList::sortByTitle() {
    head = mergeSort(head);
}
