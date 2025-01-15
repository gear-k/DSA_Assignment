#ifndef MOVIEAPP_H
#define MOVIEAPP_H

#include <string>
#include "List.h"
#include "Actor.h"
#include "Movie.h"

class MovieApp {
private:
    List<Actor> actorList;
    List<Movie> movieList;

    // For generating IDs for new actors/movies (if needed)
    int nextActorId;
    int nextMovieId;

public:
    MovieApp();

    // CSV Loaders
    void readActors(const std::string& filename);
    void readMovies(const std::string& filename);
    void readCast(const std::string& filename);

    // Menu functionalities
    void addNewActor(const std::string& name, int birthYear);
    void addNewMovie(const std::string& title, const std::string& plot, int releaseYear);

    void addActorToMovie(const std::string& actorName, const std::string& movieTitle);

    void updateActorDetails(int actorId, const std::string& newName, int newYearOfBirth);
    void updateMovieDetails(int movieId, const std::string& newTitle, const std::string& newPlot, int newReleaseYear);

    void displayAllActors() const;
    void displayAllMovies() const;
    void displayActorsByAge(int minAge, int maxAge) const;
    void displayRecentMovies() const; // last 3 years
    void displayMoviesOfActor(const std::string& actorName) const;
    void displayActorsInMovie(const std::string& movieTitle) const;
    void displayActorsKnownBy(const std::string& actorName) const;
};

#endif // MOVIEAPP_H
