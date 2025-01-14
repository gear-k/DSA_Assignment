#include "Actor.h"

Actor::Actor(const string& name, int yearOfBirth, int id)
    : id(id), name(name), yearOfBirth(yearOfBirth), age(2025 - yearOfBirth) {
}

int Actor::getId() const { return id; }
void Actor::setId(int newId) { id = newId; }

string Actor::getName() const { return name; }
int Actor::getYearOfBirth() const { return yearOfBirth; }
int Actor::getAge() const { return age; }

void Actor::setName(const string& newName) { name = newName; }
void Actor::setYearOfBirth(int newYearOfBirth) {
    yearOfBirth = newYearOfBirth;
    age = 2025 - yearOfBirth; // Recalculate age
}

void Actor::displayDetails() const {
    cout << "Actor ID: " << id << ", Name: " << name
        << ", Year of Birth: " << yearOfBirth << ", Age: " << age << endl;
}
