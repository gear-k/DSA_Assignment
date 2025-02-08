#ifndef MOVIE_APP_H
#define MOVIE_APP_H

#include "Actor.h"
#include "Movie.h"
#include "HashTable.h"
#include "RatingBST.h"
#include <string>

/***************************************************************************
 * Actor.h
 *
 * Team:            Gearoid, Cedric
 * Group:           1
 * Student IDs:     S10241866, S10241549
 *
 * Features Highlight:
 *   - Represents an actor with a fixed-size name buffer, birth year, age, and rating.
 *   - Provides safe copying and assignment operations.
 *   - Includes functions to get and set actor properties as well as display details.
 *
 ***************************************************************************/

// Manages collections of movies and actors using hash tables.
// Supports reading data from CSV files, performing CRUD operations,
// building an actor graph using BFS, and recommending movies and actors based on ratings.
class MovieApp {
private:
    HashTable<Actor> actorTable;  // Hash table storing actors by ID
    HashTable<Movie> movieTable;  // Hash table storing movies by ID

    RatingBST movieRatings;       // Binary search tree for storing movies by rating
    RatingBST actorRatings;       // Binary search tree for storing actors by rating

    int nextActorId;              // Keeps track of the next available actor ID
    int nextMovieId;              // Keeps track of the next available movie ID
    bool isAdmin;                 // Flag indicating whether admin mode is active

    // Builds the actor graph using adjacency lists
    void buildActorGraph(int actorIds[], int& actorCount, List<int>* adjacencyLists, const int MAX_ACTORS) const;

    // Finds the index of an actor in the actorIds array
    int findActorIndexInArray(int actorId, const int actorIds[], int count) const;

public:
    // Constructor initializes hash tables, next IDs, and default mode
    MovieApp();

    // --------------------------
    // Admin Mode Functions
    // --------------------------

    // Enables or disables administrator privileges
    void setAdminMode(bool admin);

    // Checks if admin mode is enabled
    bool isAdminMode() const;

    // --------------------------
    // CSV Reading Functions
    // --------------------------

    // Reads actor data from a CSV file
    void readActors(const std::string& filename);

    // Reads movie data from a CSV file
    void readMovies(const std::string& filename);

    // Reads cast data from a CSV file
    void readCast(const std::string& filename);

    // --------------------------
    // Next ID Functions
    // --------------------------

    // Returns the next available actor ID
    int getNextActorId() const;

    // Returns the next available movie ID
    int getNextMovieId() const;

    // --------------------------
    // CRUD Operations
    // --------------------------

    // Adds a new actor to the system
    void addNewActor(const std::string& name, int birthYear);

    // Adds a new movie to the system
    void addNewMovie(const std::string& title, const std::string& plot, int releaseYear);

    // Adds an actor to a movie using their IDs
    void addActorToMovieById(int actorId, int movieId, bool isAdmin);

    // Updates an actor's details
    void updateActorDetails(int actorId, const std::string& newName, int newYearOfBirth);

    // Updates a movie's details
    void updateMovieDetails(int movieId, const std::string& newTitle, const std::string& newPlot, int newReleaseYear);

    // --------------------------
    // Lookup / Condition Checks
    // --------------------------

    // Checks if an actor with the given ID exists
    bool isActorIdUsed(int id) const;

    // Checks if a movie with the given ID exists
    bool isMovieIdUsed(int id) const;

    // --------------------------
    // Find Functions
    // --------------------------

    // Finds actors by name and populates a result list
    void findActorsByName(const std::string& name, List<Actor>& result) const;

    // --------------------------
    // Display Methods
    // --------------------------

    // Displays all actors
    void displayAllActors() const;

    // Displays all movies
    void displayAllMovies() const;

    // Displays actors within a given age range
    void displayActorsByAge(int minAge, int maxAge) const;

    // Displays movies released in the last three years
    void displayRecentMovies() const;

    // Displays movies associated with a given actor ID
    void displayMoviesOfActor(int actorId) const;

    // Displays actors in a specified movie
    void displayActorsInMovie(const std::string& movieTitle) const;

    // Displays actors known by a specified actor
    void displayActorsKnownBy(const std::string& actorName) const;

    // --------------------------
    // Ratings Functions
    // --------------------------

    // Sets the rating for an actor
    void setActorRating(int actorId, int rating);

    // Sets the rating for a movie
    void setMovieRating(int movieId, int rating);

    // Recommends movies within a specified rating range
    void recommendMoviesByRating(int minRating, int maxRating) const;

    // Recommends actors within a specified rating range
    void recommendActorsByRating(int minRating, int maxRating) const;

    // --------------------------
    // Testing
    // --------------------------

    // Runs all tests for the Movie Management System
    void runAllTests();
};

#endif // MOVIE_APP_H
