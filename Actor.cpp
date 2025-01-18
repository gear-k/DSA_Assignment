#include "Actor.h"
#include <cstring>
#include <iostream>

// Default Constructor
Actor::Actor()
    : id(0), birthYear(0), age(0) {
    name[0] = '\0'; // Initialize name as an empty string
}

// Parameterized Constructor
Actor::Actor(const char* nm, int birth, int aid)
    : id(aid), birthYear(birth) {
    if (nm) {
        strncpy_s(name, sizeof(name), nm, _TRUNCATE);
    }
    else {
        name[0] = '\0'; // Initialize name as empty string
    }
    age = 2025 - birthYear; // Precompute age
}

// Copy Constructor
Actor::Actor(const Actor& other)
    : id(other.id), birthYear(other.birthYear), age(other.age) {
    strncpy_s(name, sizeof(name), other.name, _TRUNCATE);
}

// Copy Assignment Operator
Actor& Actor::operator=(const Actor& other) {
    if (this != &other) { // Prevent self-assignment
        id = other.id;
        birthYear = other.birthYear;
        age = other.age;
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
        << std::endl;
}
