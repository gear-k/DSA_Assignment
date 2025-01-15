#include "Movie.h"
#include <iostream>

Movie::Movie(const std::string& title, const std::string& plot, int releaseYear, int id)
    : id(id), title(title), plot(plot), releaseYear(releaseYear)
{
}

int Movie::getId() const {
    return id;
}

void Movie::setId(int newId) {
    id = newId;
}

std::string Movie::getTitle() const {
    return title;
}

void Movie::setTitle(const std::string& newTitle) {
    title = newTitle;
}

std::string Movie::getPlot() const {
    return plot;
}

void Movie::setPlot(const std::string& newPlot) {
    plot = newPlot;
}

int Movie::getReleaseYear() const {
    return releaseYear;
}

void Movie::setReleaseYear(int newYear) {
    releaseYear = newYear;
}

void Movie::addActor(const Actor& actor) {
    // Avoid duplicates if you wish; otherwise, just add.
    // Check if actor already in the list:
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

bool Movie::hasActor(const std::string& actorName) const {
    bool found = false;
    actors.display([&](const Actor& a) {
        if (a.getName() == actorName) {
            found = true;
            return true; // break
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
