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
    return &temp->actor;
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
