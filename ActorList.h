// ActorList.h
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

public:
    ActorList();
    ~ActorList();
    bool add(const Actor& actor);
    bool remove(int index);
    // Non-const version
    Actor* get(int index);

    // Const version (called if 'this' is const)
    const Actor* get(int index) const;

    int getLength() const;
    bool isEmpty() const;
    void displayAll() const;
    Actor* findById(int id);

    // Add this:
    void sortByName();               // <--- New function

    // Utility to check if an actor with given ID already in list
    bool containsId(int actorId) const;  // <--- For avoiding duplicates

    // Utility to check if an actor with given name is in the list
    bool containsName(const std::string& actorName) const; // optional helper
};

#endif
