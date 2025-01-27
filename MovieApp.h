// MovieApp.h
#ifndef MOVIE_APP_H
#define MOVIE_APP_H

#include "Actor.h"
#include "Movie.h"
#include "HashTable.h"
#include "RatingBST.h"
#include <string>

class MovieApp {
private:
    HashTable<Actor> actorTable;  // All actors by actorId
    HashTable<Movie> movieTable;  // All movies by movieId

    RatingBST movieRatings;
    RatingBST actorRatings;

    int nextActorId;
    int nextMovieId;
    bool isAdmin;

    // BFS adjacency-building
    void buildActorGraph(int actorIds[],
        int& actorCount,
        List<int>* adjacencyLists,
        const int MAX_ACTORS) const;
    int findActorIndexInArray(int actorId, const int actorIds[], int count) const;

public:
    MovieApp();

    // Admin Mode
    void setAdminMode(bool admin);
    bool isAdminMode() const;

    // CSV
    void readActors(const std::string& filename);
    void readMovies(const std::string& filename);
    void readCast(const std::string& filename);

    // Next IDs
    int getNextActorId() const;
    int getNextMovieId() const;

    // CRUD
    void addNewActor(const std::string& name, int birthYear);
    void addNewMovie(const std::string& title, const std::string& plot, int releaseYear);

    void addActorToMovieById(int actorId, int movieId, bool isAdmin);

    void updateActorDetails(int actorId, const std::string& newName, int newYearOfBirth);
    void updateMovieDetails(int movieId, const std::string& newTitle, const std::string& newPlot, int newReleaseYear);

    // Lookup / Condition checks
    bool isActorIdUsed(int id) const;
    bool isMovieIdUsed(int id) const;

    // Find by name
    void findActorsByName(const std::string& name, List<Actor>& result) const;

    // Display methods
    void displayAllActors() const;
    void displayAllMovies() const;
    void displayActorsByAge(int minAge, int maxAge) const;
    void displayRecentMovies() const;
    void displayMoviesOfActor(int actorId) const;
    void displayActorsInMovie(const std::string& movieTitle) const;
    void displayActorsKnownBy(const std::string& actorName) const;

    // Ratings
    void setActorRating(int actorId, int rating);
    void setMovieRating(int movieId, int rating);
    void recommendMoviesByRating(int minRating, int maxRating) const;
    void recommendActorsByRating(int minRating, int maxRating) const;

    // Test
    void runAllTests();
};

#endif // MOVIE_APP_H
