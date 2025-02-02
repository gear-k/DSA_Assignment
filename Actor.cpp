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
 *
 ***************************************************************************/

#include "Actor.h"
#include <cstring>
#include <iostream>

 /**
  * @brief Default constructor for Actor.
  *
  * Initializes the actor with default values:
  * - id: 0
  * - birthYear: 0
  * - age: 0
  * - rating: 0
  * - name: empty string
  */
Actor::Actor()
    : id(0), birthYear(0), age(0), rating(0) // initialize rating to 0
{
    name[0] = '\0';
}

/**
 * @brief Parameterized constructor for Actor.
 *
 * Initializes the actor with the provided name, birth year, and actor ID.
 * The rating is set to 0 by default, and the age is calculated based on the
 * provided birth year assuming the current year is 2025.
 *
 * @param nm Pointer to the actor's name.
 * @param birth The birth year of the actor.
 * @param aid The actor's ID.
 */
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

/**
 * @brief Copy constructor for Actor.
 *
 * Performs a deep copy of the source Actor object.
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
 * @brief Copy assignment operator for Actor.
 *
 * Checks for self-assignment and then copies all member variables
 * from the source object.
 *
 * @param other The Actor object to assign from.
 * @return Reference to the current Actor object.
 */
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

/**
 * @brief Retrieves the actor's ID.
 *
 * @return The actor's ID.
 */
int Actor::getId() const {
    return id;
}

/**
 * @brief Sets the actor's ID.
 *
 * @param newId The new actor ID.
 */
void Actor::setId(int newId) {
    id = newId;
}

/**
 * @brief Retrieves the actor's name.
 *
 * @return Pointer to the actor's name string.
 */
const char* Actor::getName() const {
    return name;
}

/**
 * @brief Sets the actor's name.
 *
 * Uses a safe string copy to update the name.
 *
 * @param newName Pointer to the new name string.
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
 * Updates the actor's birth year and recalculates the age assuming the
 * current year is 2025.
 *
 * @param newYearOfBirth The new birth year.
 */
void Actor::setBirthYear(int newYearOfBirth) {
    birthYear = newYearOfBirth;
    age = 2025 - birthYear; // Recalculate age
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
 * @brief Compares two Actor objects for equality.
 *
 * Two actors are considered equal if their IDs are the same.
 *
 * @param other The Actor object to compare with.
 * @return true if the IDs match, false otherwise.
 */

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
        << ", Rating: " << rating   // NEW: show rating
        << std::endl;
}
