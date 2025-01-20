#include "Actor.h"
#include <cstring>
#include <iostream>

// Default Constructor
Actor::Actor()
    : id(0), birthYear(0), age(0), rating(0) // initialize rating to 0
{
    name[0] = '\0';
}

// Parameterized Constructor
Actor::Actor(const char* nm, int birth, int aid)
    : id(aid), birthYear(birth), rating(0) // default rating = 0
{
    if (nm) {
        strncpy_s(name, sizeof(name), nm, _TRUNCATE);
    }
    else {
        name[0] = '\0';
    }
    age = 2025 - birthYear;
}

// Copy constructor
Actor::Actor(const Actor& other)
    : id(other.id),
    birthYear(other.birthYear),
    age(other.age),
    rating(other.rating)
{
    strncpy_s(name, sizeof(name), other.name, _TRUNCATE);
}
// Copy assignment
Actor& Actor::operator=(const Actor& other) {
    if (this != &other) {
        id = other.id;
        birthYear = other.birthYear;
        age = other.age;
        rating = other.rating; // copy rating
        strncpy_s(name, sizeof(name), other.name, _TRUNCATE);
    }
    return *this;
}

// Getters and Setters
int Actor::getId() const {
    return id;
}

void Actor::setId(int newId) {
    id = newId;
}

const char* Actor::getName() const {
    return name;
}

void Actor::setName(const char* newName) {
    if (newName) {
        strncpy_s(name, sizeof(name), newName, _TRUNCATE);
    }
}

int Actor::getBirthYear() const {
    return birthYear;
}

void Actor::setBirthYear(int newYearOfBirth) {
    birthYear = newYearOfBirth;
    age = 2025 - birthYear; // Recalculate age
}

int Actor::getAge() const {
    return age;
}

// Display actor details
void Actor::displayDetails() const {
    std::cout << "Actor ID: " << id
        << ", Name: " << name
        << ", Birth Year: " << birthYear
        << ", Age: " << age
        << ", Rating: " << rating   // NEW: show rating
        << std::endl;
}
