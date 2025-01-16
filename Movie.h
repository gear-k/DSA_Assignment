#ifndef MOVIE_H
#define MOVIE_H

#include <iostream> // for printing; remove if strictly not permitted
#include <cstring>
#include "List.h"
#include "Actor.h"

class Movie {
private:
    int   id;
    char  title[120];
    char  plot[300];
    int   releaseYear;

    // A list of Actors who star in this movie
    List<Actor> actors;

public:
    Movie();
    Movie(const char* title, const char* plot, int releaseYear, int id);

    int getId() const;
    void setId(int newId);

    const char* getTitle() const;
    void setTitle(const char* newTitle);

    const char* getPlot() const;
    void setPlot(const char* newPlot);

    int getReleaseYear() const;
    void setReleaseYear(int newYear);

    void addActor(const Actor& actor);
    bool hasActor(const char* actorName) const;

    List<Actor>& getActors();
    const List<Actor>& getActors() const;

    void displayDetails() const;
};

#endif // MOVIE_H
