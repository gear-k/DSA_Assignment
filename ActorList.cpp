#include "ActorList.h"
#include <iostream>

ActorList::ActorList() : head(nullptr), size(0) {}

ActorList::~ActorList() {
    while (head) {
        Node* temp = head;
        head = head->next;
        delete temp;
    }
}

bool ActorList::add(const Actor& actor) {
    Node* newNode = new Node{ actor, nullptr };
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

bool ActorList::remove(int index) {
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

Actor* ActorList::findById(int id) {
    Node* temp = head;
    while (temp) {
        if (temp->actor.getId() == id) return &temp->actor;
        temp = temp->next;
    }
    return nullptr;
}

Actor* ActorList::get(int index) {
    if (index < 1 || index > size) return nullptr;
    Node* temp = head;
    for (int i = 1; i < index; ++i) {
        temp = temp->next;
    }
    return &temp->actor;
}

const Actor* ActorList::get(int index) const {
    if (index < 1 || index > size) return nullptr;
    Node* temp = head;
    for (int i = 1; i < index; ++i) {
        temp = temp->next;
    }
    return &temp->actor;
}

int ActorList::getLength() const { return size; }
bool ActorList::isEmpty() const { return size == 0; }

void ActorList::displayAll() const {
    Node* temp = head;
    while (temp) {
        temp->actor.displayDetails();
        temp = temp->next;
    }
}

ActorList::Node* ActorList::mergeSortedLists(Node* left, Node* right) {
    if (!left) return right;
    if (!right) return left;

    if (left->actor.getName() < right->actor.getName()) {
        left->next = mergeSortedLists(left->next, right);
        return left;
    }
    else {
        right->next = mergeSortedLists(left, right->next);
        return right;
    }
}

ActorList::Node* ActorList::mergeSort(Node* node) {
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

void ActorList::sortByName() {
    head = mergeSort(head);
}
