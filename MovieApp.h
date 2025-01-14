#ifndef MOVIEAPP_H
#define MOVIEAPP_H

#include <string>
#include "ActorList.h"
#include "MovieList.h"

class MovieApp {
private:
    ActorList actorList;
    MovieList movieList;

public:
    void addNewActor(const std::string& name, int yearOfBirth);
    void addNewMovie(const std::string& title, const std::string& plot, int releaseYear);
    void addActorToMovie(const std::string& actorName, const std::string& movieTitle);

    void displayAllActors() const;
    void displayAllMovies() const;

    void readActors(const std::string& fileName);
    void readMovies(const std::string& fileName);
    void readCast(const std::string& fileName);

    // --- NEW FOR (g), (h), (i) ---
    void displayMoviesOfActor(const std::string& actorName) const;
    void displayActorsInMovie(const std::string& movieTitle) const;
    void displayActorsKnownBy(const std::string& actorName) const;
};

#endif
