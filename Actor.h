#ifndef ACTOR_H
#define ACTOR_H

#include <cstring>
#include <iostream>

class Actor {
private:
    char name[100];      // Fixed-size buffer for name
    int id;
    int birthYear;
    int age;

public:
    // Constructors
    Actor();
    Actor(const char* nm, int birth, int aid);
    Actor(const Actor& other);

    // Assignment Operator
    Actor& operator=(const Actor& other);

    // Getters and Setters
    int getId() const;
    void setId(int newId);

    const char* getName() const;
    void setName(const char* newName);

    int getBirthYear() const;
    void setBirthYear(int newYearOfBirth);

    int getAge() const;

    // Display Method
    void displayDetails() const;

    // Equality Operator (for List::remove)
    bool operator==(const Actor& other) const {
        return id == other.id;
    }
};

#endif // ACTOR_H
