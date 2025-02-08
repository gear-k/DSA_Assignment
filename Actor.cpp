/***************************************************************************
 * Actor.cpp
 *
 * Team:            Gearoid, Cedric
 * Group:           1
 * Student IDs:     S10241866, S10241549
 *
 * Features Highlight:
 *   - Safe string copy using strncpy_s.
 *   - Properly implemented copy constructor and copy assignment.
 *   - Age calculation based on birth year.
 *   - Display function showing actor details including rating.
 ***************************************************************************/

#include "Actor.h"
#include <cstring>
#include <iostream>

 // Default constructor initializes an actor with default values.
Actor::Actor()
    : id(0), birthYear(0), age(0), rating(0) // Initialize rating to 0
{
    name[0] = '\0'; // Empty name
}

// Constructor initializes an actor with a given name, birth year, and ID.
// The rating is set to 0 by default, and age is calculated based on the
// birth year assuming the current year is 2025.
Actor::Actor(const char* nm, int birth, int aid)
    : id(aid), birthYear(birth), rating(0) // Default rating = 0
{
    if (nm) {
        strncpy_s(name, sizeof(name), nm, _TRUNCATE);
    }
    else {
        name[0] = '\0';
    }
    age = 2025 - birthYear;
}

// Copy constructor performs a deep copy of another Actor object.
Actor::Actor(const Actor& other)
    : id(other.id),
    birthYear(other.birthYear),
    age(other.age),
    rating(other.rating)
{
    strncpy_s(name, sizeof(name), other.name, _TRUNCATE);
}

// Copy assignment operator checks for self-assignment and copies all values.
Actor& Actor::operator=(const Actor& other) {
    if (this != &other) {
        id = other.id;
        birthYear = other.birthYear;
        age = other.age;
        rating = other.rating; // Copy rating
        strncpy_s(name, sizeof(name), other.name, _TRUNCATE);
    }
    return *this;
}

// Returns the actor's unique ID.
int Actor::getId() const {
    return id;
}

// Sets the actor's ID.
void Actor::setId(int newId) {
    id = newId;
}

// Returns the actor's name.
const char* Actor::getName() const {
    return name;
}

// Updates the actor's name using safe string copy.
void Actor::setName(const char* newName) {
    if (newName) {
        strncpy_s(name, sizeof(name), newName, _TRUNCATE);
    }
}

// Returns the actor's birth year.
int Actor::getBirthYear() const {
    return birthYear;
}

// Sets the actor's birth year and recalculates age assuming the current year is 2025.
void Actor::setBirthYear(int newYearOfBirth) {
    birthYear = newYearOfBirth;
    age = 2025 - birthYear; // Recalculate age
}

// Returns the actor's age.
int Actor::getAge() const {
    return age;
}

// Displays the actor's details including ID, name, birth year, age, and rating.
void Actor::displayDetails() const {
    std::cout << "Actor ID: " << id
        << ", Name: " << name
        << ", Birth Year: " << birthYear
        << ", Age: " << age
        << ", Rating: " << rating
        << std::endl;
}
