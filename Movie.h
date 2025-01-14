#ifndef MOVIE_H
#define MOVIE_H

#include <string>
#include "ActorList.h"
using namespace std;

class Movie {
private:
    int id; // Movie's unique ID
    string title;
    string plot;
    int releaseYear;
    ActorList actors; // List of actors in the movie

public:
    Movie(const string& title, const string& plot, int releaseYear, int id);

    int getId() const;
    void setId(int newId);
    ActorList& getActors();           // Declaration of non-const version
    const ActorList& getActors() const; // Declaration of const version


    string getTitle() const;
    string getPlot() const;
    int getReleaseYear() const;

    void setTitle(const string& newTitle);
    void setPlot(const string& newPlot);
    void setReleaseYear(int newYear);

    void addActor(const Actor& actor);
    void displayDetails() const;
    bool findActorById(int actorId) const;

};

#endif
