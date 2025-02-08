#ifndef ACTOR_H
#define ACTOR_H

#include <cstring>
#include <iostream>

/***************************************************************************
 * Actor.h
 *
 * Team:            Gearoid, Cedric
 * Group:           1
 * Student IDs:     S10241866, S10241549
 *
 * Features Highlight:
 *   - Represents an actor with a fixed-size name buffer, birth year, age, and rating.
 *   - Provides safe copying and assignment operations.
 *   - Includes functions to get and set actor properties as well as display details.
 *
 ***************************************************************************/

class Actor {
private:
    char name[100];    // Fixed-size buffer for the actor's name
    int id;            // Unique identifier for the actor
    int birthYear;     // The actor's birth year
    int age;           // The actor's age (usually derived from birth year)
    int rating;        // Actor rating from 0 (no rating) to 10 (best)

public:
    // --------------------------
    // Constructors
    // --------------------------

    // Default constructor initializes an empty actor object
    Actor();

    // Constructor that initializes an actor with a name, birth year, and ID
    Actor(const char* nm, int birth, int aid);

    // Copy constructor to create a duplicate actor from an existing one
    Actor(const Actor& other);

    // Overloaded assignment operator to safely assign values between actors
    Actor& operator=(const Actor& other);

    // --------------------------
    // Getters and Setters
    // --------------------------

    // Returns the actor's unique ID
    int getId() const;

    // Sets a new ID for the actor
    void setId(int newId);

    // Returns the actor's name
    const char* getName() const;

    // Updates the actor's name, ensuring it's copied safely
    void setName(const char* newName);

    // Returns the actor's birth year
    int getBirthYear() const;

    // Sets a new birth year for the actor
    void setBirthYear(int newYearOfBirth);

    // Returns the actor's age
    int getAge() const;

    // Displays all details of the actor (name, ID, birth year, age, and rating)
    void displayDetails() const;

    // --------------------------
    // Operators
    // --------------------------

    // Overloads the equality operator to compare actors by their ID
    bool operator==(const Actor& other) const {
        return id == other.id;
    }

    // Returns the actor's rating
    int getRating() const { return rating; }

    // Sets the actor's rating (expected to be between 0 and 10)
    void setRating(int r) { rating = r; }
};

#endif // ACTOR_H
