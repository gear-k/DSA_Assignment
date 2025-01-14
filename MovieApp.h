#ifndef MOVIEAPP_H
#define MOVIEAPP_H

#include <string>
#include "ActorList.h"
#include "MovieList.h"

class MovieApp {
private:
    ActorList actorList;  // Actor list for the app
    MovieList movieList;  // Movie list for the app

public:
    void addNewActor(const std::string& name, int yearOfBirth);
    void addNewMovie(const std::string& title, const std::string& plot, int releaseYear);
    void addActorToMovie(const std::string& actorName, const std::string& movieTitle);
    
    void updateActorDetails(int actorId, const std::string& newName, int newYearOfBirth);
    void updateMovieDetails(int movieId, const std::string& newTitle, const std::string& newPlot, int newReleaseYear);

    void displayAllActors() const;
    void displayAllMovies() const;

    void displayActorsByAge(int minAge, int maxAge) const;
    void displayRecentMovies() const;

    void displayMoviesOfActor(const std::string& actorName) const;
    void displayActorsInMovie(const std::string& movieTitle) const;
    void displayActorsKnownBy(const std::string& actorName) const;

    void readActors(const std::string& fileName);
    void readMovies(const std::string& fileName);
    void readCast(const std::string& fileName);
};

#endif
