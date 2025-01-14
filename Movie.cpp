#include "Movie.h"
#include <iostream>

// Constructor implementation in Movie.cpp
Movie::Movie(const string& title, const string& plot, int releaseYear, int id)
    : id(id), title(title), plot(plot), releaseYear(releaseYear) {}

int Movie::getId() const {
    return id;
}

void Movie::setId(int newId) {
    id = newId;
}

string Movie::getTitle() const {
    return title;
}

string Movie::getPlot() const {
    return plot;
}

int Movie::getReleaseYear() const {
    return releaseYear;
}

void Movie::setTitle(const string& newTitle) {
    title = newTitle;
}

void Movie::setPlot(const string& newPlot) {
    plot = newPlot;
}

void Movie::setReleaseYear(int newYear) {
    releaseYear = newYear;
}

void Movie::addActor(const Actor& actor) {
    actors.add(actor);
}

void Movie::displayDetails() const {
    cout << "Movie ID: " << id << ", Title: " << title << ", Release Year: " << releaseYear << endl;
    cout << "Actors in this movie:" << endl;
    actors.displayAll();
}
