#include "Actor.h"

Actor::Actor(const std::string& name, int birthYear, int id)
    : id(id), name(name), birthYear(birthYear), age(2025 - birthYear)
{
}

int Actor::getId() const {
    return id;
}

void Actor::setId(int newId) {
    id = newId;
}

std::string Actor::getName() const {
    return name;
}

void Actor::setName(const std::string& newName) {
    name = newName;
}

int Actor::getBirthYear() const {
    return birthYear;
}

void Actor::setBirthYear(int newYearOfBirth) {
    birthYear = newYearOfBirth;
    age = 2025 - birthYear;  // recalc
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

