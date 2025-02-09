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
 *   - Supports reading data from CSV files, performing CRUD operations,
 *     building an actor graph using BFS, and recommending movies and actors
 *     based on ratings.
 *
 ***************************************************************************/

 /**
  * @brief The MovieApp class manages movies and actors.
  *
  * This class uses hash tables to store and manage collections of movies and actors.
  * It supports reading data from CSV files, performing CRUD operations, building
  * an actor graph via BFS, and recommending movies and actors based on ratings.
  */
class MovieApp {
private:
    HashTable<Actor> actorTable;  ///< Hash table storing actors by ID.
    HashTable<Movie> movieTable;  ///< Hash table storing movies by ID.
    RatingBST movieRatings;       ///< Binary search tree for storing movies by rating.
    RatingBST actorRatings;       ///< Binary search tree for storing actors by rating.
    int nextActorId;              ///< Next available actor ID.
    int nextMovieId;              ///< Next available movie ID.
    bool isAdmin;                 ///< Flag indicating whether admin mode is active.

    /**
     * @brief Builds the actor graph using adjacency lists.
     *
     * @param actorIds An array to store actor IDs.
     * @param actorCount Reference to an integer to hold the total number of actors.
     * @param adjacencyLists An array of adjacency lists representing the actor graph.
     * @param MAX_ACTORS Maximum number of actors expected.
     */
    void buildActorGraph(int actorIds[], int& actorCount, List<int>* adjacencyLists, const int MAX_ACTORS) const;

    /**
     * @brief Finds the index of an actor in the actorIds array.
     *
     * @param actorId The actor's ID to search for.
     * @param actorIds The array of actor IDs.
     * @param count Total number of actor IDs in the array.
     * @return int The index of the actor if found; otherwise, -1.
     */
    int findActorIndexInArray(int actorId, const int actorIds[], int count) const;

public:
    // --------------------------
    // Constructor
    // --------------------------

    /**
     * @brief Constructs a new MovieApp object.
     *
     * Initializes hash tables, next IDs, and sets the default mode.
     */
    MovieApp();

    // --------------------------
    // Admin Mode Functions
    // --------------------------

    /**
     * @brief Enables or disables administrator privileges.
     *
     * @param admin Set to true to enable admin mode, false to disable.
     */
    void setAdminMode(bool admin);

    /**
     * @brief Checks if administrator mode is enabled.
     *
     * @return true if admin mode is active, false otherwise.
     */
    bool isAdminMode() const;

    // --------------------------
    // CSV Reading Functions
    // --------------------------

    /**
     * @brief Reads actor data from a CSV file.
     *
     * @param filename The path to the CSV file containing actor data.
     */
    void readActors(const std::string& filename);

    /**
     * @brief Reads movie data from a CSV file.
     *
     * @param filename The path to the CSV file containing movie data.
     */
    void readMovies(const std::string& filename);

    /**
     * @brief Reads cast data from a CSV file.
     *
     * @param filename The path to the CSV file containing cast data.
     */
    void readCast(const std::string& filename);

    // --------------------------
    // Next ID Functions
    // --------------------------

    /**
     * @brief Retrieves the next available actor ID.
     *
     * @return int The next actor ID.
     */
    int getNextActorId() const;

    /**
     * @brief Retrieves the next available movie ID.
     *
     * @return int The next movie ID.
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
     * @param actorId The ID of the actor.
     * @param movieId The ID of the movie.
     * @param isAdmin A flag indicating if the operation is allowed (admin mode).
     */
    void addActorToMovieById(int actorId, int movieId, bool isAdmin);

    /**
     * @brief Updates the details of an existing actor.
     *
     * @param actorId The ID of the actor.
     * @param newName The new name for the actor.
     * @param newYearOfBirth The new birth year for the actor.
     */
    void updateActorDetails(int actorId, const std::string& newName, int newYearOfBirth);

    /**
     * @brief Updates the details of an existing movie.
     *
     * @param movieId The ID of the movie.
     * @param newTitle The new title for the movie.
     * @param newPlot The new plot description for the movie.
     * @param newReleaseYear The new release year for the movie.
     */
    void updateMovieDetails(int movieId, const std::string& newTitle, const std::string& newPlot, int newReleaseYear);

    // --------------------------
    // Lookup / Condition Checks
    // --------------------------

    /**
     * @brief Checks if an actor with the specified ID exists.
     *
     * @param id The actor ID to check.
     * @return true if the actor exists, false otherwise.
     */
    bool isActorIdUsed(int id) const;

    /**
     * @brief Checks if a movie with the specified ID exists.
     *
     * @param id The movie ID to check.
     * @return true if the movie exists, false otherwise.
     */
    bool isMovieIdUsed(int id) const;

    // --------------------------
    // Find Functions
    // --------------------------

    /**
     * @brief Finds actors by name and populates a result list.
     *
     * @param name The name to search for.
     * @param result The list that will be populated with matching actors.
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
     * @brief Displays actors whose ages fall within the specified range.
     *
     * @param minAge The minimum age (inclusive).
     * @param maxAge The maximum age (inclusive).
     */
    void displayActorsByAge(int minAge, int maxAge) const;

    /**
     * @brief Displays movies that were released in the last three years.
     */
    void displayRecentMovies() const;

    /**
     * @brief Displays movies associated with the specified actor.
     *
     * @param actorId The ID of the actor.
     */
    void displayMoviesOfActor(int actorId) const;

    /**
     * @brief Displays actors appearing in the specified movie.
     *
     * @param movieTitle The title of the movie.
     */
    void displayActorsInMovie(const std::string& movieTitle) const;

    /**
     * @brief Displays actors known by the specified actor.
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
     * @param actorId The ID of the actor.
     * @param rating The rating value (expected range: 1 to 10).
     */
    void setActorRating(int actorId, int rating);

    /**
     * @brief Sets the rating for a movie.
     *
     * @param movieId The ID of the movie.
     * @param rating The rating value (expected range: 1 to 10).
     */
    void setMovieRating(int movieId, int rating);

    /**
     * @brief Recommends movies whose ratings fall within the specified range.
     *
     * @param minRating The minimum rating (inclusive).
     * @param maxRating The maximum rating (inclusive).
     */
    void recommendMoviesByRating(int minRating, int maxRating) const;

    /**
     * @brief Recommends actors whose ratings fall within the specified range.
     *
     * @param minRating The minimum rating (inclusive).
     * @param maxRating The maximum rating (inclusive).
     */
    void recommendActorsByRating(int minRating, int maxRating) const;

};

#endif // MOVIE_APP_H
