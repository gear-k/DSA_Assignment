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
    int rating; // Range: 0 (no rating) to 10 (max)

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

    // NEW: Rating getter & setter
    int getRating() const { return rating; }
    void setRating(int r) { rating = r; }
};

#endif // ACTOR_H
