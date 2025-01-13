#pragma once
#include <string>
#include "ActorList.h"

class Movie {
private:
    string title;
    string plot;
    int releaseYear;
    ActorList actors;

public:
    Movie(const string& title, const string& plot, int releaseYear);

    // Accessors
    string getTitle() const;
    string getPlot() const;
    int getReleaseYear() const;

    // Mutators
    void setTitle(const string& newTitle);
    void setPlot(const string& newPlot);
    void setReleaseYear(int newYear);

    // Actor management
    void addActor(const Actor& actor);
    void displayDetails() const;
};
