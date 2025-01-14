#ifndef ACTOR_H
#define ACTOR_H

#include <string>
#include <iostream>
using namespace std;

class Actor {
private:
    int id; // Actor's unique ID
    string name;
    int yearOfBirth;
    int age;

public:
    Actor(const string& name, int yearOfBirth, int id);

    int getId() const;
    void setId(int newId);

    string getName() const;
    int getYearOfBirth() const;
    int getAge() const;

    void setName(const string& newName);
    void setYearOfBirth(int newYearOfBirth);

    void displayDetails() const;
};

#endif
