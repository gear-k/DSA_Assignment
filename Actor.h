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
    Actor();
    Actor(const char* name, int birthYear, int id);

    int getId() const;
    void setId(int newId);

    const char* getName() const;
    void setName(const char* newName);

    int getBirthYear() const;
    void setBirthYear(int newYearOfBirth);

    int getAge() const;

    void displayDetails() const; // For printing
};

#endif // ACTOR_H
