#include "Movie.h"
#include <cstring> // for strncpy_s

Movie::Movie()
    : id(0), releaseYear(0)
{
    title[0] = '\0';
    plot[0] = '\0';
}

Movie::Movie(const char* ttl, const char* plt, int rYear, int mid)
    : id(mid), releaseYear(rYear)
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

// Copy Constructor
Movie::Movie(const Movie& other)
    : id(other.id), releaseYear(other.releaseYear), actors(other.actors)
{
    strncpy_s(title, sizeof(title), other.title, _TRUNCATE);
    strncpy_s(plot, sizeof(plot), other.plot, _TRUNCATE);
}

// Copy Assignment Operator
Movie& Movie::operator=(const Movie& other) {
    if (this != &other) {
        id = other.id;
        releaseYear = other.releaseYear;
        actors = other.actors;

        strncpy_s(title, sizeof(title), other.title, _TRUNCATE);
        strncpy_s(plot, sizeof(plot), other.plot, _TRUNCATE);
    }
    return *this;
}

int Movie::getId() const {
    return id;
}

void Movie::setId(int newId) {
    id = newId;
}

const char* Movie::getTitle() const {
    return title;
}

void Movie::setTitle(const char* newTitle) {
    // Use strncpy_s for safer string copy
    if (newTitle) {
        strncpy_s(title, sizeof(title), newTitle, _TRUNCATE);
    }
}

const char* Movie::getPlot() const {
    return plot;
}

void Movie::setPlot(const char* newPlot) {
    // Use strncpy_s for safer string copy
    if (newPlot) {
        strncpy_s(plot, sizeof(plot), newPlot, _TRUNCATE);
    }
}

int Movie::getReleaseYear() const {
    return releaseYear;
}

void Movie::setReleaseYear(int newYear) {
    releaseYear = newYear;
}

void Movie::addActor(const Actor& actor) {
    bool exists = false;
    actors.display([&](const Actor& a) {
        if (a.getId() == actor.getId()) {
            exists = true;
            return true; // break
        }
        return false;
        });
    if (!exists) {
        actors.add(actor);
    }
}

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
        std::cout << "Comparing Actor Name: " << aName << " with " << actorName << std::endl;
        if (std::strcmp(aName, actorName) == 0) {
            found = true;
            return true; // Break out of the loop
        }
        return false;
        });
    return found;
}


List<Actor>& Movie::getActors() {
    return actors;
}

const List<Actor>& Movie::getActors() const {
    return actors;
}

void Movie::displayDetails() const {
    std::cout << "Movie ID: " << id
        << ", Title: " << title
        << ", Year: " << releaseYear
        << std::endl;
}
