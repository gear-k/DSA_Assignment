#ifndef MOVIE_H
#define MOVIE_H

#include "Actor.h"
#include "List.h"
#include <iostream>
#include <cstring>

/***************************************************************************
 * Movie.h
 *
 * Team:            Gearoid, Cedric
 * Group:           1
 * Student IDs:     S10241866, S10241549
 *
 * Features Highlight:
 *   - Represents a movie with a unique identifier, title, plot description,
 *     release year, rating, and a list of actors.
 *   - Provides operations to manage movie details, including adding actors,
 *     updating information, and displaying details.
 *
 ***************************************************************************/

 /**
  * @brief The Movie class represents a movie with its associated details.
  */
class Movie {
private:
    int id;                  ///< Unique movie identifier.
    char title[100];         ///< Title of the movie.
    char plot[500];          ///< Short description of the movie's plot.
    int releaseYear;         ///< Year the movie was released.
    List<Actor> actors;      ///< List of actors in the movie.
    int rating;              ///< Movie rating (0 to 10).

public:
    // --------------------------
    // Comparison Operators
    // --------------------------

    /**
     * @brief Compares two movies based on their ID.
     *
     * @param other The movie to compare with.
     * @return true if the movies have the same ID, false otherwise.
     */
    bool operator==(const Movie& other) const;

    // --------------------------
    // Constructors and Destructor
    // --------------------------

    /**
     * @brief Default constructor.
     *
     * Initializes an empty movie with default values.
     */
    Movie();

    /**
     * @brief Constructs a Movie with specified details.
     *
     * @param ttl The title of the movie.
     * @param plt The plot description of the movie.
     * @param rYear The release year of the movie.
     * @param mid The unique movie identifier.
     */
    Movie(const char* ttl, const char* plt, int rYear, int mid);

    /**
     * @brief Copy constructor.
     *
     * Creates a deep copy of another movie.
     *
     * @param other The movie to copy.
     */
    Movie(const Movie& other);

    /**
     * @brief Assignment operator.
     *
     * Copies the details of one movie to another.
     *
     * @param other The movie to assign from.
     * @return A reference to the current movie after assignment.
     */
    Movie& operator=(const Movie& other);

    /**
     * @brief Destructor.
     *
     * Default destructor.
     */
    ~Movie() = default;

    // --------------------------
    // Getters and Setters
    // --------------------------

    /**
     * @brief Retrieves the movie's ID.
     *
     * @return The unique identifier of the movie.
     */
    int getId() const;

    /**
     * @brief Sets a new ID for the movie.
     *
     * @param newId The new unique identifier to assign.
     */
    void setId(int newId);

    /**
     * @brief Retrieves the movie's title.
     *
     * @return A constant pointer to the movie's title.
     */
    const char* getTitle() const;

    /**
     * @brief Updates the movie's title.
     *
     * @param newTitle The new title to assign.
     */
    void setTitle(const char* newTitle);

    /**
     * @brief Retrieves the movie's plot.
     *
     * @return A constant pointer to the movie's plot description.
     */
    const char* getPlot() const;

    /**
     * @brief Updates the movie's plot.
     *
     * @param newPlot The new plot description to assign.
     */
    void setPlot(const char* newPlot);

    /**
     * @brief Retrieves the release year of the movie.
     *
     * @return The release year.
     */
    int getReleaseYear() const;

    /**
     * @brief Updates the release year of the movie.
     *
     * @param newYear The new release year to assign.
     */
    void setReleaseYear(int newYear);

    /**
     * @brief Retrieves the movie's rating.
     *
     * @return The rating of the movie.
     */
    int getRating() const { return rating; }

    /**
     * @brief Sets a new rating for the movie.
     *
     * @param r The new rating (expected range: 0 to 10).
     */
    void setRating(int r) { rating = r; }

    // --------------------------
    // Actor Management
    // --------------------------

    /**
     * @brief Adds an actor to the movie's actor list.
     *
     * If the actor is not already present, they will be added to the list.
     *
     * @param actor The actor to add.
     */
    void addActor(const Actor& actor);

    /**
     * @brief Checks if an actor with the specified name is in the movie.
     *
     * @param actorName The name of the actor to check.
     * @return true if the actor is present, false otherwise.
     */
    bool hasActor(const char* actorName) const;

    /**
     * @brief Checks if an actor with the specified ID is in the movie.
     *
     * @param actorId The unique identifier of the actor.
     * @return true if the actor is present, false otherwise.
     */
    bool hasActor(int actorId) const;

    // --------------------------
    // Access to Actors List
    // --------------------------

    /**
     * @brief Retrieves a reference to the list of actors in the movie (modifiable version).
     *
     * @return A reference to the list of actors.
     */
    List<Actor>& getActors();

    /**
     * @brief Retrieves a reference to the list of actors in the movie (read-only version).
     *
     * @return A constant reference to the list of actors.
     */
    const List<Actor>& getActors() const;

    // --------------------------
    // Display Methods
    // --------------------------

    /**
     * @brief Displays the movie details.
     *
     * Outputs the movie's ID, title, release year, and rating to the standard output.
     */
    void displayDetails() const;
};

#endif // MOVIE_H
