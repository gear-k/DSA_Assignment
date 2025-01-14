#include "MovieList.h"
#include <iostream>

MovieList::MovieList() : head(nullptr), size(0) {}

MovieList::~MovieList() {
    while (!isEmpty()) {
        remove(1);
    }
}

bool MovieList::add(const Movie& movie) {
    Node* newNode = new Node{ movie, nullptr };
    if (size == 0) {
        head = newNode;
    }
    else {
        Node* temp = head;
        while (temp->next != nullptr) {
            temp = temp->next;
        }
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
        for (int i = 1; i < index; i++) {
            prev = temp;
            temp = temp->next;
        }
        prev->next = temp->next;
    }
    delete temp;
    size--;
    return true;
}

Movie* MovieList::get(int index) {
    if (index < 1 || index > size) return nullptr;
    Node* temp = head;
    for (int i = 1; i < index; i++) {
        temp = temp->next;
    }
    return &temp->movie; // pointer to *mutable* Movie
}

const Movie* MovieList::get(int index) const {
    if (index < 1 || index > size) return nullptr;
    Node* temp = head;
    for (int i = 1; i < index; i++) {
        temp = temp->next;
    }
    return &temp->movie; // pointer to *const* Movie
}


int MovieList::getLength() const {
    return size;
}

bool MovieList::isEmpty() const {
    return (size == 0);
}

void MovieList::displayAll() const {
    Node* temp = head;
    while (temp != nullptr) {
        temp->movie.displayDetails();
        temp = temp->next;
    }
}

Movie* MovieList::findById(int id) {
    Node* current = head;
    while (current != nullptr) {
        if (current->movie.getId() == id) {
            return &current->movie;
        }
        current = current->next;
    }
    return nullptr;
}

// -------------------- NEW CODE --------------------

// Sort movies by title (ascending) using bubble sort on linked list
void MovieList::sortByTitle() {
    if (!head || !head->next) {
        // 0 or 1 node, nothing to sort
        return;
    }

    bool swapped;
    do {
        swapped = false;
        Node* current = head;
        while (current->next != nullptr) {
            if (current->movie.getTitle() > current->next->movie.getTitle()) {
                // swap the movie objects
                Movie temp = current->movie;
                current->movie = current->next->movie;
                current->next->movie = temp;
                swapped = true;
            }
            current = current->next;
        }
    } while (swapped);
}


