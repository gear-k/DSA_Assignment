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

 // Default constructor initializes a movie with default values.
Movie::Movie()
    : id(0), releaseYear(0), rating(0) // rating defaults to 0
{
    title[0] = '\0';
    plot[0] = '\0';
}

// Compares two Movie objects based on their IDs.
bool Movie::operator==(const Movie& other) const {
    return this->id == other.id;
}

// Parameterized constructor initializes a movie with the given title, plot, release year, and ID.
// The rating is set to 0 by default.
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

// Copy constructor creates a deep copy of another movie.
Movie::Movie(const Movie& other)
    : id(other.id),
    releaseYear(other.releaseYear),
    actors(other.actors),
    rating(other.rating)
{
    strncpy_s(title, sizeof(title), other.title, _TRUNCATE);
    strncpy_s(plot, sizeof(plot), other.plot, _TRUNCATE);
}

// Copy assignment operator ensures deep copy of data while avoiding self-assignment.
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

// Returns the movie ID.
int Movie::getId() const {
    return id;
}

// Sets the movie ID.
void Movie::setId(int newId) {
    id = newId;
}

// Returns the movie title.
const char* Movie::getTitle() const {
    return title;
}

// Updates the movie title using safe string copy.
void Movie::setTitle(const char* newTitle) {
    if (newTitle) {
        strncpy_s(title, sizeof(title), newTitle, _TRUNCATE);
    }
}

// Returns the movie plot.
const char* Movie::getPlot() const {
    return plot;
}

// Updates the movie plot using safe string copy.
void Movie::setPlot(const char* newPlot) {
    if (newPlot) {
        strncpy_s(plot, sizeof(plot), newPlot, _TRUNCATE);
    }
}

// Returns the movie's release year.
int Movie::getReleaseYear() const {
    return releaseYear;
}

// Sets the movie's release year.
void Movie::setReleaseYear(int newYear) {
    releaseYear = newYear;
}

// Adds an actor to the movie's cast, ensuring no duplicates.
void Movie::addActor(const Actor& actor) {
    bool exists = false;
    actors.display([&](const Actor& a) {
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

// Checks if the movie has an actor with the given name.
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
            return true; // exit loop early
        }
        return false;
        });
    return found;
}

// Checks if the movie has an actor with the given ID.
bool Movie::hasActor(int actorId) const {
    bool found = false;
    actors.display([&](const Actor& a) {
        if (a.getId() == actorId) {
            found = true;
            return true; // exit loop early
        }
        return false;
        });
    return found;
}

// Returns a reference to the list of actors in the movie.
List<Actor>& Movie::getActors() {
    return actors;
}

// Returns a const reference to the list of actors in the movie.
const List<Actor>& Movie::getActors() const {
    return actors;
}

// Displays the details of the movie, including its ID, title, release year, and rating.
void Movie::displayDetails() const {
    std::cout << "Movie ID: " << id
        << ", Title: " << title
        << ", Plot: " << plot
        << ", Year: " << releaseYear
        << ", Rating: " << rating
        << std::endl;
}
