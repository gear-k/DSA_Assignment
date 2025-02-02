// MovieApp.h
#ifndef MOVIE_APP_H
#define MOVIE_APP_H

#include "Actor.h"
#include "Movie.h"
#include "HashTable.h"
#include "RatingBST.h"
#include <string>

/***************************************************************************
 * MovieApp.h
 *
 * Team:            Gearoid, Cedric
 * Group:           1
 * Student IDs:     S10241866, S10241549
 *
 * Features Highlight:
 *   - Manages collections of movies and actors using hash tables.
 *   - Supports CSV file input for actors, movies, and cast data.
 *   - Provides CRUD operations along with rating functionality.
 *   - Builds an actor graph to manage connections between actors.
 *
 ***************************************************************************/

 /**
  * @brief The MovieApp class encapsulates the Movie Management System.
  *
  * It provides functionality for managing movies and actors,
  * including reading data from CSV files, performing CRUD operations,
  * building an actor graph using BFS, and recommending movies and actors
  * based on ratings.
  */
class MovieApp {
private:
    HashTable<Actor> actorTable;  // All actors indexed by actorId
    HashTable<Movie> movieTable;   // All movies indexed by movieId

    RatingBST movieRatings;        // BST for storing movies by rating
    RatingBST actorRatings;        // BST for storing actors by rating

    int nextActorId;               // Next available actor ID
    int nextMovieId;               // Next available movie ID
    bool isAdmin;                  // Flag for admin mode

    /**
     * @brief Builds the actor graph from the stored actors.
     *
     * Populates the actorIds array and adjacencyLists for graph traversal.
     *
     * @param actorIds Array to store actor IDs.
     * @param actorCount Reference to an integer to store the total count of actors.
     * @param adjacencyLists Array of lists representing the adjacency lists.
     * @param MAX_ACTORS Maximum number of actors.
     */
    void buildActorGraph(int actorIds[],
        int& actorCount,
        List<int>* adjacencyLists,
        const int MAX_ACTORS) const;

    /**
     * @brief Finds the index of an actor in the actorIds array.
     *
     * @param actorId The actor ID to search for.
     * @param actorIds Array of actor IDs.
     * @param count Number of elements in actorIds.
     * @return The index of the actor if found; otherwise, -1.
     */
    int findActorIndexInArray(int actorId, const int actorIds[], int count) const;

public:
    /**
     * @brief Constructs a new MovieApp object.
     *
     * Initializes hash tables, next IDs, and default mode.
     */
    MovieApp();

    // --------------------------
    // Admin Mode Functions
    // --------------------------

    /**
     * @brief Sets the admin mode.
     *
     * Enables or disables administrator privileges.
     *
     * @param admin True to enable admin mode; false to disable.
     */
    void setAdminMode(bool admin);

    /**
     * @brief Checks if admin mode is enabled.
     *
     * @return True if in admin mode; false otherwise.
     */
    bool isAdminMode() const;

    // --------------------------
    // CSV Reading Functions
    // --------------------------

    /**
     * @brief Reads actor data from a CSV file.
     *
     * @param filename The path to the CSV file.
     */
    void readActors(const std::string& filename);

    /**
     * @brief Reads movie data from a CSV file.
     *
     * @param filename The path to the CSV file.
     */
    void readMovies(const std::string& filename);

    /**
     * @brief Reads cast data from a CSV file.
     *
     * @param filename The path to the CSV file.
     */
    void readCast(const std::string& filename);

    // --------------------------
    // Next ID Functions
    // --------------------------

    /**
     * @brief Retrieves the next available actor ID.
     *
     * @return The next actor ID.
     */
    int getNextActorId() const;

    /**
     * @brief Retrieves the next available movie ID.
     *
     * @return The next movie ID.
     */
    int getNextMovieId() const;

    // --------------------------
    // CRUD Operations
    // --------------------------

    /**
     * @brief Adds a new actor to the system.
     *
     * @param name The name of the actor.
     * @param birthYear The birth year of the actor.
     */
    void addNewActor(const std::string& name, int birthYear);

    /**
     * @brief Adds a new movie to the system.
     *
     * @param title The title of the movie.
     * @param plot The plot description of the movie.
     * @param releaseYear The release year of the movie.
     */
    void addNewMovie(const std::string& title, const std::string& plot, int releaseYear);

    /**
     * @brief Adds an actor to a movie using their IDs.
     *
     * @param actorId The actor's ID.
     * @param movieId The movie's ID.
     * @param isAdmin Flag indicating whether the operation is performed in admin mode.
     */
    void addActorToMovieById(int actorId, int movieId, bool isAdmin);

    /**
     * @brief Updates an actor's details.
     *
     * @param actorId The actor's ID.
     * @param newName The new name for the actor.
     * @param newYearOfBirth The new birth year for the actor.
     */
    void updateActorDetails(int actorId, const std::string& newName, int newYearOfBirth);

    /**
     * @brief Updates a movie's details.
     *
     * @param movieId The movie's ID.
     * @param newTitle The new title for the movie.
     * @param newPlot The new plot for the movie.
     * @param newReleaseYear The new release year for the movie.
     */
    void updateMovieDetails(int movieId, const std::string& newTitle, const std::string& newPlot, int newReleaseYear);

    // --------------------------
    // Lookup / Condition Checks
    // --------------------------

    /**
     * @brief Checks if an actor with the given ID exists.
     *
     * @param id The actor ID to check.
     * @return True if the actor exists; false otherwise.
     */
    bool isActorIdUsed(int id) const;

    /**
     * @brief Checks if a movie with the given ID exists.
     *
     * @param id The movie ID to check.
     * @return True if the movie exists; false otherwise.
     */
    bool isMovieIdUsed(int id) const;

    // --------------------------
    // Find Functions
    // --------------------------

    /**
     * @brief Finds actors by name.
     *
     * Populates the result list with all actors matching the given name.
     *
     * @param name The name to search for.
     * @param result The list to be populated with matching actors.
     */
    void findActorsByName(const std::string& name, List<Actor>& result) const;

    // --------------------------
    // Display Methods
    // --------------------------

    /**
     * @brief Displays all actors.
     */
    void displayAllActors() const;

    /**
     * @brief Displays all movies.
     */
    void displayAllMovies() const;

    /**
     * @brief Displays actors within a given age range.
     *
     * @param minAge The minimum age (inclusive).
     * @param maxAge The maximum age (inclusive).
     */
    void displayActorsByAge(int minAge, int maxAge) const;

    /**
     * @brief Displays movies released in the last three years.
     */
    void displayRecentMovies() const;

    /**
     * @brief Displays movies associated with the given actor ID.
     *
     * @param actorId The actor's ID.
     */
    void displayMoviesOfActor(int actorId) const;

    /**
     * @brief Displays actors in a specified movie.
     *
     * @param movieTitle The title of the movie.
     */
    void displayActorsInMovie(const std::string& movieTitle) const;

    /**
     * @brief Displays actors known by a specified actor.
     *
     * @param actorName The name of the actor.
     */
    void displayActorsKnownBy(const std::string& actorName) const;

    // --------------------------
    // Ratings Functions
    // --------------------------

    /**
     * @brief Sets the rating for an actor.
     *
     * @param actorId The actor's ID.
     * @param rating The new rating to set.
     */
    void setActorRating(int actorId, int rating);

    /**
     * @brief Sets the rating for a movie.
     *
     * @param movieId The movie's ID.
     * @param rating The new rating to set.
     */
    void setMovieRating(int movieId, int rating);

    /**
     * @brief Recommends movies within a specified rating range.
     *
     * @param minRating The minimum rating (inclusive).
     * @param maxRating The maximum rating (inclusive).
     */
    void recommendMoviesByRating(int minRating, int maxRating) const;

    /**
     * @brief Recommends actors within a specified rating range.
     *
     * @param minRating The minimum rating (inclusive).
     * @param maxRating The maximum rating (inclusive).
     */
    void recommendActorsByRating(int minRating, int maxRating) const;

    // --------------------------
    // Testing
    // --------------------------

    /**
     * @brief Runs all tests for the Movie Management System.
     */
    void runAllTests();
};

#endif // MOVIE_APP_H
