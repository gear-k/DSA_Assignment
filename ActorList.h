#ifndef ACTORLIST_H
#define ACTORLIST_H

#include "Actor.h"

class ActorList {
private:
    struct Node {
        Actor actor;
        Node* next;
    };
    Node* head;
    int size;

    // Moved to private and made accessible within the class
    Node* mergeSortedLists(Node* left, Node* right);
    Node* mergeSort(Node* node);

public:
    ActorList();
    ~ActorList();
    bool add(const Actor& actor);
    bool remove(int index);
    Actor* findById(int id);
    Actor* get(int index);
    const Actor* get(int index) const;

    int getLength() const;
    bool isEmpty() const;
    void displayAll() const;

    void sortByName(); // Sort actors by name
};

#endif
