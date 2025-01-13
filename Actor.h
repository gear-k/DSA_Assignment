#pragma once
#include <string>
#include <iostream>
using namespace std;

class Actor {
private:
    string name;
    int yearOfBirth;
    int age;

public:
    Actor(const string& name, int yearOfBirth);

    // Accessors
    string getName() const;
    int getYearOfBirth() const;
    int getAge() const;

    // Mutators
    void setName(const string& newName);
    void setYearOfBirth(int newYearOfBirth);

    // Display
    void displayDetails() const;
};
