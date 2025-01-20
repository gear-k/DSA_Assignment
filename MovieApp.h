#ifndef MOVIE_APP_H
#define MOVIE_APP_H

#include "Actor.h"
#include "Movie.h"
#include "List.h"

class MovieApp {
private:
    List<Actor> actorList;
    List<Movie> movieList;

    int nextActorId;
    int nextMovieId;

    // For BFS adjacency, we need:
    // - A way to store every actor's ID in an array
    // - A way to store adjacency as "actorIndex -> list of actorIndices"
    // We'll build these on-the-fly in "displayActorsKnownBy" to avoid using STL.

    // Helper: build an in-memory graph of adjacency
    // We'll store arrays: actorIds[], adjacencyLists[] (where adjacencyLists[i] is a List<int>)
    void buildActorGraph(
        int actorIds[],
        int& actorCount,
        List<int>* adjacencyLists,
        const int MAX_ACTORS
    ) const;

    int findActorIndexInArray(int actorId, const int actorIds[], int count) const;

public:
    MovieApp();

    // CSV reading
    void readActors(const std::string& filename);
    void readMovies(const std::string& filename);
    void readCast(const std::string& filename);

    int getNextActorId() const;
    int getNextMovieId() const;


    // "Add New" items
    void addNewActor(const std::string& name, int birthYear);
    void addNewMovie(const std::string& title, const std::string& plot, int releaseYear);



    void addActorToMovieById(int actorId, int movieId);

    // Update actor/movie
    void updateActorDetails(int actorId, const std::string& newName, int newYearOfBirth);
    void updateMovieDetails(int movieId, const std::string& newTitle, const std::string& newPlot, int newReleaseYear);

    // Display
    void displayAllActors() const;
    void displayAllMovies() const;
    void displayActorsByAge(int minAge, int maxAge) const;
    void displayRecentMovies() const;
    void displayMoviesOfActor(const std::string& actorName) const;
    void displayActorsInMovie(const std::string& movieTitle) const;

    // The BFS version (2-level acquaintances):
    void displayActorsKnownBy(const std::string& actorName) const;
    
    void setActorRating(int actorId, int rating);
    void setMovieRating(int movieId, int rating);

    bool isActorIdUsed(int id) const;
    bool isMovieIdUsed(int id) const;


    // New Test Function
    void runAllTests();
};

#endif // MOVIE_APP_H
