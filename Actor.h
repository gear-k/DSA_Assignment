#ifndef ACTOR_H
#define ACTOR_H

#include <iostream> // for printing; remove if strictly not permitted.
#include <cstring>

class Actor {
private:
    int   id;
    char  name[100];   // C-style storage of name
    int   birthYear;
    int   age;         // Age computed as 2025 - birthYear

public:
    // Default Constructor
    Actor();

    // Parameterized Constructor
    Actor(const char* name, int birthYear, int id);

    // Copy Constructor
    Actor(const Actor& other);

    // Copy Assignment Operator
    Actor& operator=(const Actor& other);

    // Getters and Setters
    int getId() const;
    void setId(int newId);

    const char* getName() const;
    void setName(const char* newName);

    int getBirthYear() const;
    void setBirthYear(int newYearOfBirth);

    int getAge() const;

    // Display actor details for printing or debugging
    void displayDetails() const;
};

#endif // ACTOR_H
