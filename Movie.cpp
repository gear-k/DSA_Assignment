/***************************************************************************
 * Movie.cpp
 *
 * Team:            Gearoid, Cedric
 * Group:           1
 * Student IDs:     S10241866, S10241549
 *
 * Features Highlight:
 *   - Safe string copy using strncpy_s.
 *   - Copy constructor and copy assignment operator implemented.
 *   - Actor list management and duplicate prevention.
 ***************************************************************************/

#include "Movie.h"
#include <cstring> // for strncpy_s
#include <iostream> // for std::cout, std::cerr

 /**
  * @brief Default constructor for Movie.
  *
  * Initializes a Movie object with default values:
  * - id = 0, releaseYear = 0, rating = 0.
  * - title and plot are set to empty strings.
  */
Movie::Movie()
    : id(0), releaseYear(0), rating(0)
{
    title[0] = '\0';
    plot[0] = '\0';
}

/**
 * @brief Equality operator for Movie.
 *
 * Compares two Movie objects based on their IDs.
 *
 * @param other The Movie object to compare against.
 * @return true if both movies have the same ID; false otherwise.
 */
bool Movie::operator==(const Movie& other) const {
    return this->id == other.id;
}

/**
 * @brief Parameterized constructor for Movie.
 *
 * Initializes a Movie object with the provided title, plot, release year, and ID.
 * The rating is set to 0 by default.
 *
 * @param ttl The title of the movie.
 * @param plt The plot of the movie.
 * @param rYear The release year of the movie.
 * @param mid The movie ID.
 */
Movie::Movie(const char* ttl, const char* plt, int rYear, int mid)
    : id(mid), releaseYear(rYear), rating(0)
{
    if (ttl) {
        strncpy_s(title, sizeof(title), ttl, _TRUNCATE);
    }
    else {
        title[0] = '\0';
    }
    if (plt) {
        strncpy_s(plot, sizeof(plot), plt, _TRUNCATE);
    }
    else {
        plot[0] = '\0';
    }
}

/**
 * @brief Copy constructor for Movie.
 *
 * Creates a deep copy of another Movie object.
 *
 * @param other The Movie object to copy.
 */
Movie::Movie(const Movie& other)
    : id(other.id),
    releaseYear(other.releaseYear),
    actors(other.actors),
    rating(other.rating)
{
    strncpy_s(title, sizeof(title), other.title, _TRUNCATE);
    strncpy_s(plot, sizeof(plot), other.plot, _TRUNCATE);
}

/**
 * @brief Copy assignment operator for Movie.
 *
 * Performs a deep copy of the other Movie object while avoiding self-assignment.
 *
 * @param other The Movie object to copy.
 * @return A reference to the current Movie object.
 */
Movie& Movie::operator=(const Movie& other) {
    if (this != &other) {
        id = other.id;
        releaseYear = other.releaseYear;
        actors = other.actors;
        rating = other.rating; // copy rating
        strncpy_s(title, sizeof(title), other.title, _TRUNCATE);
        strncpy_s(plot, sizeof(plot), other.plot, _TRUNCATE);
    }
    return *this;
}

/**
 * @brief Gets the movie ID.
 *
 * @return The movie's unique identifier.
 */
int Movie::getId() const {
    return id;
}

/**
 * @brief Sets the movie ID.
 *
 * @param newId The new movie ID.
 */
void Movie::setId(int newId) {
    id = newId;
}

/**
 * @brief Gets the movie title.
 *
 * @return A constant character pointer to the movie title.
 */
const char* Movie::getTitle() const {
    return title;
}

/**
 * @brief Sets the movie title.
 *
 * Uses safe string copy to update the title.
 *
 * @param newTitle The new title for the movie.
 */
void Movie::setTitle(const char* newTitle) {
    if (newTitle) {
        strncpy_s(title, sizeof(title), newTitle, _TRUNCATE);
    }
}

/**
 * @brief Gets the movie plot.
 *
 * @return A constant character pointer to the movie plot.
 */
const char* Movie::getPlot() const {
    return plot;
}

/**
 * @brief Sets the movie plot.
 *
 * Uses safe string copy to update the plot.
 *
 * @param newPlot The new plot for the movie.
 */
void Movie::setPlot(const char* newPlot) {
    if (newPlot) {
        strncpy_s(plot, sizeof(plot), newPlot, _TRUNCATE);
    }
}

/**
 * @brief Gets the movie's release year.
 *
 * @return The release year of the movie.
 */
int Movie::getReleaseYear() const {
    return releaseYear;
}

/**
 * @brief Sets the movie's release year.
 *
 * @param newYear The new release year for the movie.
 */
void Movie::setReleaseYear(int newYear) {
    releaseYear = newYear;
}

/**
 * @brief Adds an actor to the movie's cast.
 *
 * Prevents adding duplicate actors by checking if an actor with the same ID
 * already exists in the cast.
 *
 * @param actor The Actor object to add.
 */
void Movie::addActor(const Actor& actor) {
    bool exists = false;
    actors.forEach([&](const Actor& a) {
        if (a.getId() == actor.getId()) {
            exists = true;
            return true; // exit loop early
        }
        return false;
        });

    if (!exists) {
        actors.add(actor);
    }
}

/**
 * @brief Checks if the movie has an actor with the given name.
 *
 * @param actorName A C-string representing the actor's name.
 * @return true if an actor with the specified name exists in the movie; false otherwise.
 */
bool Movie::hasActor(const char* actorName) const {
    if (actorName == nullptr) {
        std::cerr << "Error: actorName is nullptr." << std::endl;
        return false;
    }

    bool found = false;
    actors.forEach([&](const Actor& a) {
        const char* aName = a.getName();
        if (aName == nullptr) {
            std::cerr << "Error: Actor name is nullptr." << std::endl;
            return false;
        }
        if (std::strcmp(aName, actorName) == 0) {
            found = true;
            return true; // exit loop early
        }
        return false;
        });
    return found;
}

/**
 * @brief Checks if the movie has an actor with the given ID.
 *
 * @param actorId The actor's ID to check.
 * @return true if an actor with the specified ID exists in the movie; false otherwise.
 */
bool Movie::hasActor(int actorId) const {
    bool found = false;
    actors.forEach([&](const Actor& a) {
        if (a.getId() == actorId) {
            found = true;
            return true; // exit loop early
        }
        return false;
        });
    return found;
}

/**
 * @brief Gets a reference to the list of actors in the movie.
 *
 * @return A reference to the List of Actor objects.
 */
List<Actor>& Movie::getActors() {
    return actors;
}

/**
 * @brief Gets a constant reference to the list of actors in the movie.
 *
 * @return A constant reference to the List of Actor objects.
 */
const List<Actor>& Movie::getActors() const {
    return actors;
}

/**
 * @brief Displays the details of the movie.
 *
 * Outputs the movie's ID, title, plot, release year, and rating to the standard output.
 */
void Movie::displayDetails() const {
    std::cout << "Movie ID: " << id
        << ", Title: " << title
        << ", Plot: " << plot
        << ", Year: " << releaseYear
        << ", Rating: " << rating
        << std::endl;
}
