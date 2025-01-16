#include "Actor.h"
#include <cstring>

Actor::Actor()
    : id(0), birthYear(0), age(0)
{
    name[0] = '\0';
}

Actor::Actor(const char* nm, int birth, int aid)
    : id(aid), birthYear(birth)
{
    // Use strncpy_s for safer string copy
    if (nm) {
        strncpy_s(name, sizeof(name), nm, _TRUNCATE);
    }
    else {
        name[0] = '\0';
    }

    age = 2025 - birthYear; // Precompute
}

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
    // Use strncpy_s for safer string copy
    if (newName) {
        strncpy_s(name, sizeof(name), newName, _TRUNCATE);
    }
}

int Actor::getBirthYear() const {
    return birthYear;
}

void Actor::setBirthYear(int newYearOfBirth) {
    birthYear = newYearOfBirth;
    age = 2025 - birthYear;
}

int Actor::getAge() const {
    return age;
}

void Actor::displayDetails() const {
    std::cout << "Actor ID: " << id
        << ", Name: " << name
        << ", Birth Year: " << birthYear
        << ", Age: " << age
        << std::endl;
}
