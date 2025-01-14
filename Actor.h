#ifndef ACTOR_H
#define ACTOR_H

#include <string>
#include <iostream>
using namespace std;

class Actor {
private:
    int id; // Unique Actor ID
    string name; // Actor's name
    int yearOfBirth; // Year of birth
    int age; // Calculated age

public:
    Actor() = default; // Default constructor
    Actor(const string& name, int yearOfBirth, int id);

    // Getter and setter functions
    int getId() const;
    void setId(int newId);

    string getName() const;
    int getYearOfBirth() const;
    int getAge() const;

    void setName(const string& newName);
    void setYearOfBirth(int newYearOfBirth);

    void displayDetails() const; // Display actor details
};

#endif
