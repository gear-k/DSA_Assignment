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

 /**
  * @brief Default constructor.
  *
  * Initializes an Actor object with default values:
  * - id = 0
  * - birthYear = 0
  * - age = 0
  * - rating = 0
  * - name set to an empty string.
  */
Actor::Actor()
    : id(0), birthYear(0), age(0), rating(0)
{
    name[0] = '\0';
}

/**
 * @brief Parameterized constructor.
 *
 * Initializes an Actor object with the provided name, birth year, and id.
 * The rating is set to 0 by default, and the age is calculated based on
 * the birth year assuming the current year is 2025.
 *
 * @param nm   A C-string representing the actor's name.
 * @param birth The actor's birth year.
 * @param aid   The actor's unique identifier.
 */
Actor::Actor(const char* nm, int birth, int aid)
    : id(aid), birthYear(birth), rating(0)
{
    if (nm) {
        strncpy_s(name, sizeof(name), nm, _TRUNCATE);
    }
    else {
        name[0] = '\0';
    }
    age = 2025 - birthYear;
}

/**
 * @brief Copy constructor.
 *
 * Creates a new Actor object as a deep copy of an existing Actor object.
 *
 * @param other The Actor object to copy from.
 */
Actor::Actor(const Actor& other)
    : id(other.id),
    birthYear(other.birthYear),
    age(other.age),
    rating(other.rating)
{
    strncpy_s(name, sizeof(name), other.name, _TRUNCATE);
}

/**
 * @brief Copy assignment operator.
 *
 * Assigns the values from one Actor object to this object (deep copy).
 * Checks for self-assignment before copying.
 *
 * @param other The Actor object to copy from.
 * @return Reference to the current Actor object.
 */
Actor& Actor::operator=(const Actor& other) {
    if (this != &other) {
        id = other.id;
        birthYear = other.birthYear;
        age = other.age;
        rating = other.rating;
        strncpy_s(name, sizeof(name), other.name, _TRUNCATE);
    }
    return *this;
}

/**
 * @brief Gets the actor's unique ID.
 *
 * @return The actor's ID.
 */
int Actor::getId() const {
    return id;
}

/**
 * @brief Sets the actor's unique ID.
 *
 * @param newId The new ID to assign to the actor.
 */
void Actor::setId(int newId) {
    id = newId;
}

/**
 * @brief Retrieves the actor's name.
 *
 * @return A C-string containing the actor's name.
 */
const char* Actor::getName() const {
    return name;
}

/**
 * @brief Updates the actor's name.
 *
 * Safely updates the actor's name using strncpy_s.
 *
 * @param newName A C-string containing the new name.
 */
void Actor::setName(const char* newName) {
    if (newName) {
        strncpy_s(name, sizeof(name), newName, _TRUNCATE);
    }
}

/**
 * @brief Retrieves the actor's birth year.
 *
 * @return The actor's birth year.
 */
int Actor::getBirthYear() const {
    return birthYear;
}

/**
 * @brief Sets the actor's birth year.
 *
 * Updates the actor's birth year and recalculates the age assuming the current year is 2025.
 *
 * @param newYearOfBirth The new birth year to assign.
 */
void Actor::setBirthYear(int newYearOfBirth) {
    birthYear = newYearOfBirth;
    age = 2025 - birthYear;
}

/**
 * @brief Retrieves the actor's age.
 *
 * @return The actor's age.
 */
int Actor::getAge() const {
    return age;
}

/**
 * @brief Displays the actor's details.
 *
 * Outputs the actor's ID, name, birth year, age, and rating to the standard output.
 */
void Actor::displayDetails() const {
    std::cout << "Actor ID: " << id
        << ", Name: " << name
        << ", Birth Year: " << birthYear
        << ", Age: " << age
        << ", Rating: " << rating
        << std::endl;
}
