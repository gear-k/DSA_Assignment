#pragma once
#include "MovieList.h"

class MovieApp {
private:
    ActorList actorList;
    MovieList movieList;

public:
    void addNewActor(const string& name, int yearOfBirth);
    void addNewMovie(const string& title, const string& plot, int releaseYear);
    void addActorToMovie(const string& actorName, const string& movieTitle);
    void displayAllActors() const;
    void displayAllMovies() const;
};
