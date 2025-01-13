#include "Actor.h"

Actor::Actor(const string& name, int yearOfBirth)
    : name(name), yearOfBirth(yearOfBirth), age(2025 - yearOfBirth) {
}

string Actor::getName() const {
    return name;
}

int Actor::getYearOfBirth() const {
    return yearOfBirth;
}

int Actor::getAge() const {
    return age;
}

void Actor::setName(const string& newName) {
    name = newName;
}

void Actor::setYearOfBirth(int newYearOfBirth) {
    yearOfBirth = newYearOfBirth;
    age = 2025 - yearOfBirth;
}

void Actor::displayDetails() const {
    cout << "Actor: " << name << ", Year of Birth: " << yearOfBirth << ", Age: " << age << endl;
}
