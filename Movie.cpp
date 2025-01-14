#include "Movie.h"
#include <iostream>

Movie::Movie(const std::string& title, const std::string& plot, int releaseYear, int id)
    : id(id), title(title), plot(plot), releaseYear(releaseYear) {
}

int Movie::getId() const {
    return id;
}

std::string Movie::getTitle() const {
    return title;
}

void Movie::setTitle(const std::string& newTitle) {
    title = newTitle;
}

void Movie::setPlot(const std::string& newPlot) {
    plot = newPlot;
}

void Movie::setReleaseYear(int newYear) {
    releaseYear = newYear;
}

ActorList& Movie::getActors() {
    return actors;  // Non-const version
}

const ActorList& Movie::getActors() const {
    return actors;  // Const version
}

int Movie::getReleaseYear() const {
    return releaseYear;
}

void Movie::addActor(const Actor& actor) {
    actors.add(actor);
}

void Movie::displayDetails() const {
    std::cout << "Movie ID: " << id << ", Title: " << title
        << ", Release Year: " << releaseYear << std::endl;
    std::cout << "Actors in this movie:" << std::endl;
    actors.displayAll();
}
