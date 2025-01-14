#include "ActorList.h"
#include <iostream>

ActorList::ActorList() : head(nullptr), size(0) {}

ActorList::~ActorList() {
    while (!isEmpty()) {
        remove(1);
    }
}

bool ActorList::add(const Actor& actor) {
    Node* newNode = new Node{ actor, nullptr };
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

bool ActorList::remove(int index) {
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


Actor* ActorList::get(int index) {
    if (index < 1 || index > size) return nullptr;
    Node* temp = head;
    for (int i = 1; i < index; i++) {
        temp = temp->next;
    }
    return &temp->actor; // pointer to *mutable* Actor
}

const Actor* ActorList::get(int index) const {
    if (index < 1 || index > size) return nullptr;
    Node* temp = head;
    for (int i = 1; i < index; i++) {
        temp = temp->next;
    }
    return &temp->actor; // pointer to *const* Actor
}


int ActorList::getLength() const {
    return size;
}

bool ActorList::isEmpty() const {
    return size == 0;
}

void ActorList::displayAll() const {
    Node* temp = head;
    while (temp != nullptr) {
        temp->actor.displayDetails();
        temp = temp->next;
    }
}

Actor* ActorList::findById(int id) {
    Node* current = head;
    while (current != nullptr) {
        if (current->actor.getId() == id) {
            return &current->actor;
        }
        current = current->next;
    }
    return nullptr;
}

// -------------------- NEW CODE --------------------

// Sort actors by name (ascending) using bubble sort on linked list
void ActorList::sortByName() {
    if (!head || !head->next) {
        // 0 or 1 node, nothing to sort
        return;
    }

    bool swapped;
    do {
        swapped = false;
        Node* current = head;
        while (current->next != nullptr) {
            if (current->actor.getName() > current->next->actor.getName()) {
                // swap the actor objects
                Actor temp = current->actor;
                current->actor = current->next->actor;
                current->next->actor = temp;
                swapped = true;
            }
            current = current->next;
        }
    } while (swapped);
}

// Check if the list already contains an actor with a given ID
bool ActorList::containsId(int actorId) const {
    Node* current = head;
    while (current != nullptr) {
        if (current->actor.getId() == actorId) {
            return true;
        }
        current = current->next;
    }
    return false;
}
