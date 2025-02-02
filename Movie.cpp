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
 *
 ***************************************************************************/

#include "Movie.h"
#include <cstring> // for strncpy_s

 /**
  * @brief Default constructor for Movie.
  *
  * Initializes the movie with default values. The rating is set to 0, and the
  * title and plot strings are initialized as empty.
  */
Movie::Movie()
    : id(0), releaseYear(0), rating(0) // rating defaults to 0
{
    title[0] = '\0';
    plot[0] = '\0';
}

/**
 * @brief Equality comparison operator for Movie.
 *
 * Compares two Movie objects based on their IDs.
 *
 * @param other The other Movie object to compare with.
 * @return True if the IDs are equal; false otherwise.
 */
bool Movie::operator==(const Movie& other) const {
    return this->id == other.id;
}

/**
 * @brief Parameterized constructor for Movie.
 *
 * Initializes the movie with the provided title, plot, release year, and ID.
 * The rating is initialized to 0.
 *
 * @param ttl Pointer to the title string.
 * @param plt Pointer to the plot string.
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
 * Performs a deep copy of the source movie.
 *
 * @param other The source Movie object to copy from.
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
 * Checks for self-assignment and then copies all member variables from the
 * source object.
 *
 * @param other The source Movie object to assign from.
 * @return Reference to the current Movie object.
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
 * @brief Retrieves the movie ID.
 *
 * @return The movie ID.
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
 * @brief Retrieves the title of the movie.
 *
 * @return Pointer to the title string.
 */
const char* Movie::getTitle() const {
    return title;
}

/**
 * @brief Sets the title of the movie.
 *
 * Uses a safe string copy to update the title.
 *
 * @param newTitle Pointer to the new title string.
 */
void Movie::setTitle(const char* newTitle) {
    if (newTitle) {
        strncpy_s(title, sizeof(title), newTitle, _TRUNCATE);
    }
}

/**
 * @brief Retrieves the plot of the movie.
 *
 * @return Pointer to the plot string.
 */
const char* Movie::getPlot() const {
    return plot;
}

/**
 * @brief Sets the plot of the movie.
 *
 * Uses a safe string copy to update the plot.
 *
 * @param newPlot Pointer to the new plot string.
 */
void Movie::setPlot(const char* newPlot) {
    if (newPlot) {
        strncpy_s(plot, sizeof(plot), newPlot, _TRUNCATE);
    }
}

/**
 * @brief Retrieves the release year of the movie.
 *
 * @return The release year.
 */
int Movie::getReleaseYear() const {
    return releaseYear;
}

/**
 * @brief Sets the release year of the movie.
 *
 * @param newYear The new release year.
 */
void Movie::setReleaseYear(int newYear) {
    releaseYear = newYear;
}

/**
 * @brief Adds an actor to the movie's cast.
 *
 * Checks if the actor already exists in the list by comparing actor IDs.
 * If not present, adds the actor to the cast.
 *
 * @param actor The Actor to be added.
 */
void Movie::addActor(const Actor& actor) {
    bool exists = false;
    actors.display([&](const Actor& a) {
        if (a.getId() == actor.getId()) {
            exists = true;
            return true; // break out of the loop
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
 * @param actorName Pointer to the actor's name to search for.
 * @return True if the actor is found; false otherwise.
 */
bool Movie::hasActor(const char* actorName) const {
    if (actorName == nullptr) {
        std::cerr << "Error: actorName is nullptr." << std::endl;
        return false;
    }

    bool found = false;
    actors.display([&](const Actor& a) {
        const char* aName = a.getName();
        if (aName == nullptr) {
            std::cerr << "Error: Actor name is nullptr." << std::endl;
            return false;
        }
        if (std::strcmp(aName, actorName) == 0) {
            found = true;
            return true; // break out of the loop
        }
        return false;
        });
    return found;
}

/**
 * @brief Checks if the movie has an actor with the given ID.
 *
 * @param actorId The actor ID to search for.
 * @return True if the actor is found; false otherwise.
 */
bool Movie::hasActor(int actorId) const {
    bool found = false;
    actors.display([&](const Actor& a) {
        if (a.getId() == actorId) {
            found = true;
            return true; // break out of the loop
        }
        return false;
        });
    return found;
}

/**
 * @brief Retrieves a reference to the list of actors in the movie.
 *
 * @return Reference to the List of Actor objects.
 */
List<Actor>& Movie::getActors() {
    return actors;
}

/**
 * @brief Retrieves a const reference to the list of actors in the movie.
 *
 * @return Const reference to the List of Actor objects.
 */
const List<Actor>& Movie::getActors() const {
    return actors;
}

/**
 * @brief Displays the details of the movie.
 *
 * Prints the movie ID, title, release year, and rating to standard output.
 */
void Movie::displayDetails() const {
    std::cout << "Movie ID: " << id
        << ", Title: " << title
        << ", Year: " << releaseYear
        << ", Rating: " << rating
        << std::endl;
}
