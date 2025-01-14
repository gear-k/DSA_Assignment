#pragma once
#include "Actor.h"

class ActorList {
private:
    struct Node {
        Actor actor;
        Node* next;
    };
    Node* head;
    int size;

public:
    ActorList();
    ~ActorList();

    bool add(const Actor& actor);
    bool remove(int index);
    Actor* get(int index);
    int getLength() const;
    bool isEmpty() const;
    void displayAll() const;
    Actor* findById(int id);
};
