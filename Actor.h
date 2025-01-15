#ifndef ACTOR_H
#define ACTOR_H

#include <string>
#include <iostream>

class Actor {
private:
    int id;
    std::string name;
    int birthYear;
    int age; // for convenience

public:
    Actor() : id(0), birthYear(0), age(0) {}
    Actor(const std::string& name, int birthYear, int id);

    int getId() const;
    void setId(int newId);

    std::string getName() const;
    void setName(const std::string& newName);

    int getBirthYear() const;
    void setBirthYear(int newYearOfBirth);

    int getAge() const;

    void displayDetails() const;

    // For storing in List<Actor>, we need an equality check
    bool operator==(const Actor& other) const {
        return (this->id == other.id);
    }

    // So that List<Actor>::display() can do std::cout << actor
    friend std::ostream& operator<<(std::ostream& os, const Actor& actor) {
        os << "[Actor ID=" << actor.id
            << ", Name=\"" << actor.name
            << "\", BirthYear=" << actor.birthYear
            << ", Age=" << actor.age << "]";
        return os;
    }
};

#endif // ACTOR_H
