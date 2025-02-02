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
  * @brief The Actor class represents an actor with basic information.
  *
  * The class stores the actor's name, unique identifier, birth year, age, and rating.
  * The age is typically calculated from the birth year. The rating ranges from 0 (no
  * rating) to 10 (maximum rating).
  */
class Actor {
private:
    char name[100];    ///< Fixed-size buffer for the actor's name.
    int id;            ///< Unique identifier for the actor.
    int birthYear;     ///< The actor's birth year.
    int age;           ///< The actor's age (typically derived from the birth year).
    int rating;        ///< Actor rating (range: 0 to 10).

public:
    // --------------------------
    // Constructors
    // --------------------------



    /**
     * @brief Default constructor.
     *
     * Initializes the actor with default values. The name is set to an empty string,
     * and numerical values (id, birthYear, age, rating) are initialized to 0.
     */
    Actor();

    /**
     * @brief Parameterized constructor.
     *
     * Initializes the actor with the provided name, birth year, and actor ID.
     * The rating is set to 0 by default, and age should be calculated accordingly.
     *
     * @param nm Pointer to the actor's name.
     * @param birth The actor's birth year.
     * @param aid The actor's unique identifier.
     */
    Actor(const char* nm, int birth, int aid);

    /**
     * @brief Copy constructor.
     *
     * Creates a new Actor object as a deep copy of another.
     *
     * @param other The Actor object to copy.
     */
    Actor(const Actor& other);

    // --------------------------
    // Assignment Operator
    // --------------------------

    /**
     * @brief Assignment operator.
     *
     * Assigns values from another Actor object to this one.
     *
     * @param other The Actor object to assign from.
     * @return Reference to the current Actor object.
     */
    Actor& operator=(const Actor& other);

    // --------------------------
    // Getters and Setters
    // --------------------------

    /**
     * @brief Retrieves the actor's ID.
     *
     * @return The actor's unique identifier.
     */
    int getId() const;

    /**
     * @brief Sets the actor's ID.
     *
     * @param newId The new unique identifier.
     */
    void setId(int newId);

    /**
     * @brief Retrieves the actor's name.
     *
     * @return A pointer to the actor's name string.
     */
    const char* getName() const;

    /**
     * @brief Sets the actor's name.
     *
     * Copies the new name into the fixed-size buffer.
     *
     * @param newName Pointer to the new name string.
     */
    void setName(const char* newName);

    /**
     * @brief Retrieves the actor's birth year.
     *
     * @return The birth year.
     */
    int getBirthYear() const;

    /**
     * @brief Sets the actor's birth year.
     *
     * Updates the actor's birth year and recalculates the age as needed.
     *
     * @param newYearOfBirth The new birth year.
     */
    void setBirthYear(int newYearOfBirth);

    /**
     * @brief Retrieves the actor's age.
     *
     * @return The actor's age.
     */
    int getAge() const;

    // --------------------------
    // Display Method
    // --------------------------

    /**
     * @brief Displays the actor's details.
     *
     * Outputs the actor's ID, name, birth year, age, and rating to standard output.
     */
    void displayDetails() const;

    // --------------------------
    // Operators
    // --------------------------

    /**
     * @brief Equality operator.
     *
     * Compares two Actor objects based on their unique identifiers.
     *
     * @param other The Actor object to compare with.
     * @return True if both actors have the same ID; false otherwise.
     */
    bool operator==(const Actor& other) const {
        return id == other.id;
    }

    // --------------------------
    // Rating Accessors
    // --------------------------

    /**
     * @brief Retrieves the actor's rating.
     *
     * @return The rating value (0 to 10).
     */
    int getRating() const { return rating; }

    /**
     * @brief Sets the actor's rating.
     *
     * @param r The new rating value (0 to 10).
     */
    void setRating(int r) { rating = r; }
};

#endif // ACTOR_H
