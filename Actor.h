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

 /**
  * @brief The Actor class represents an actor with a name, ID, birth year, age, and rating.
  */
class Actor {
private:
    char name[100];    ///< Fixed-size buffer for the actor's name
    int id;            ///< Unique identifier for the actor
    int birthYear;     ///< The actor's birth year
    int age;           ///< The actor's age (usually derived from birth year)
    int rating;        ///< Actor rating from 0 (no rating) to 10 (best)

public:
    // --------------------------
    // Constructors
    // --------------------------

    /**
     * @brief Default constructor.
     *
     * Initializes an empty actor object.
     */
    Actor();

    /**
     * @brief Constructs an Actor with a given name, birth year, and unique ID.
     *
     * @param nm The actor's name.
     * @param birth The actor's birth year.
     * @param aid The actor's unique identifier.
     */
    Actor(const char* nm, int birth, int aid);

    /**
     * @brief Copy constructor.
     *
     * Creates a duplicate actor from an existing one.
     *
     * @param other The Actor object to copy.
     */
    Actor(const Actor& other);

    /**
     * @brief Assignment operator.
     *
     * Safely assigns values from one Actor to another.
     *
     * @param other The Actor object to assign from.
     * @return A reference to this Actor after assignment.
     */
    Actor& operator=(const Actor& other);

    // --------------------------
    // Getters and Setters
    // --------------------------

    /**
     * @brief Retrieves the actor's unique ID.
     *
     * @return The actor's ID.
     */
    int getId() const;

    /**
     * @brief Sets a new ID for the actor.
     *
     * @param newId The new ID to assign.
     */
    void setId(int newId);

    /**
     * @brief Retrieves the actor's name.
     *
     * @return A constant pointer to the actor's name.
     */
    const char* getName() const;

    /**
     * @brief Safely updates the actor's name.
     *
     * @param newName The new name to assign.
     */
    void setName(const char* newName);

    /**
     * @brief Retrieves the actor's birth year.
     *
     * @return The actor's birth year.
     */
    int getBirthYear() const;

    /**
     * @brief Sets a new birth year for the actor.
     *
     * @param newYearOfBirth The new birth year to assign.
     */
    void setBirthYear(int newYearOfBirth);

    /**
     * @brief Retrieves the actor's age.
     *
     * @return The actor's age.
     */
    int getAge() const;

    /**
     * @brief Displays all details of the actor.
     *
     * Prints the actor's name, ID, birth year, age, and rating.
     */
    void displayDetails() const;

    // --------------------------
    // Operators
    // --------------------------

    /**
     * @brief Compares this actor with another actor.
     *
     * Two actors are considered equal if they have the same ID.
     *
     * @param other The actor to compare with.
     * @return True if both actors have the same ID, false otherwise.
     */
    bool operator==(const Actor& other) const {
        return id == other.id;
    }

    /**
     * @brief Retrieves the actor's rating.
     *
     * @return The actor's rating (expected to be between 0 and 10).
     */
    int getRating() const { return rating; }

    /**
     * @brief Sets the actor's rating.
     *
     * @param r The new rating to assign (expected to be between 0 and 10).
     */
    void setRating(int r) { rating = r; }
};

#endif // ACTOR_H
