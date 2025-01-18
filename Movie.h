#ifndef MOVIE_H
#define MOVIE_H

#include "Actor.h"
#include "List.h"
#include <iostream>
#include <cstring>

class Movie {
private:
    int id;
    char title[100];
    char plot[500];
    int releaseYear;
    List<Actor> actors;

public:
    // Constructors
    Movie();
    Movie(const char* ttl, const char* plt, int rYear, int mid);

    // Copy Constructor
    Movie(const Movie& other);

    // Copy Assignment Operator
    Movie& operator=(const Movie& other);

    // Destructor
    ~Movie() = default;

    // Getters and Setters
    int getId() const;
    void setId(int newId);

    const char* getTitle() const;
    void setTitle(const char* newTitle); // Ensure this signature matches

    const char* getPlot() const;
    void setPlot(const char* newPlot);

    int getReleaseYear() const;
    void setReleaseYear(int newYear); // Ensure this signature matches

    // Actor Management
    void addActor(const Actor& actor);
    bool hasActor(const char* actorName) const;

    // Access to Actors List
    List<Actor>& getActors();
    const List<Actor>& getActors() const;

    // Display Details
    void displayDetails() const;
};

#endif // MOVIE_H
