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
 *   - Represents a movie with title, plot, release year, and rating.
 *   - Manages a list of actors associated with the movie.
 *   - Implements safe copy and assignment operations.
 *
 ***************************************************************************/

 /**
  * @brief The Movie class represents a movie with details such as title, plot,
  * release year, and rating. It also stores a list of actors that are part of the movie.
  */
class Movie {
private:
    int id;                  ///< Unique movie identifier
    char title[100];         ///< Title of the movie
    char plot[500];          ///< Plot description of the movie
    int releaseYear;         ///< Year the movie was released
    List<Actor> actors;      ///< List of actors in the movie
    int rating;              ///< Movie rating in the range 0 to 10

public:
    // Compare two movies (modify the criteria as needed)
    bool operator==(const Movie& other) const;
    // --------------------------
    // Constructors and Destructor
    // --------------------------

    /**
     * @brief Default constructor.
     *
     * Initializes the movie with default values. The rating is set to 0.
     */
    Movie();

    /**
     * @brief Parameterized constructor.
     *
     * Initializes the movie with a title, plot, release year, and movie ID.
     * The rating is set to 0 by default.
     *
     * @param ttl Pointer to the title string.
     * @param plt Pointer to the plot string.
     * @param rYear The release year.
     * @param mid The movie ID.
     */
    Movie(const char* ttl, const char* plt, int rYear, int mid);

    /**
     * @brief Copy constructor.
     *
     * Creates a new movie as a deep copy of the provided movie.
     *
     * @param other The movie object to copy from.
     */
    Movie(const Movie& other);

    /**
     * @brief Copy assignment operator.
     *
     * Assigns values from another movie object to this one.
     *
     * @param other The movie object to assign from.
     * @return Reference to the current movie.
     */
    Movie& operator=(const Movie& other);

    /**
     * @brief Default destructor.
     */
    ~Movie() = default;

    // --------------------------
    // Getters and Setters
    // --------------------------

    /**
     * @brief Retrieves the movie's ID.
     *
     * @return The movie's ID.
     */
    int getId() const;

    /**
     * @brief Sets the movie's ID.
     *
     * @param newId The new movie ID.
     */
    void setId(int newId);

    /**
     * @brief Retrieves the movie's title.
     *
     * @return Pointer to the movie's title string.
     */
    const char* getTitle() const;

    /**
     * @brief Sets the movie's title.
     *
     * Uses safe string copy to update the title.
     *
     * @param newTitle Pointer to the new title string.
     */
    void setTitle(const char* newTitle);

    /**
     * @brief Retrieves the movie's plot.
     *
     * @return Pointer to the movie's plot string.
     */
    const char* getPlot() const;

    /**
     * @brief Sets the movie's plot.
     *
     * Uses safe string copy to update the plot.
     *
     * @param newPlot Pointer to the new plot string.
     */
    void setPlot(const char* newPlot);

    /**
     * @brief Retrieves the release year of the movie.
     *
     * @return The release year.
     */
    int getReleaseYear() const;

    /**
     * @brief Sets the movie's release year.
     *
     * @param newYear The new release year.
     */
    void setReleaseYear(int newYear);

    /**
     * @brief Retrieves the movie's rating.
     *
     * @return The current movie rating.
     */
    int getRating() const { return rating; }

    /**
     * @brief Sets the movie's rating.
     *
     * @param r The new rating to set.
     */
    void setRating(int r) { rating = r; }

    // --------------------------
    // Actor Management
    // --------------------------

    /**
     * @brief Adds an actor to the movie.
     *
     * Checks if the actor already exists in the list before adding.
     *
     * @param actor The actor to add.
     */
    void addActor(const Actor& actor);

    /**
     * @brief Checks if the movie has an actor with the specified name.
     *
     * @param actorName Pointer to the actor name string.
     * @return True if the actor exists; false otherwise.
     */
    bool hasActor(const char* actorName) const;

    /**
     * @brief Checks if the movie has an actor with the specified ID.
     *
     * @param actorId The actor ID to search for.
     * @return True if the actor exists; false otherwise.
     */
    bool hasActor(int actorId) const;

    // --------------------------
    // Access to Actors List
    // --------------------------

    /**
     * @brief Provides access to the list of actors (non-const version).
     *
     * @return Reference to the list of actors.
     */
    List<Actor>& getActors();

    /**
     * @brief Provides access to the list of actors (const version).
     *
     * @return Const reference to the list of actors.
     */
    const List<Actor>& getActors() const;

    // --------------------------
    // Display Methods
    // --------------------------

    /**
     * @brief Displays movie details to the standard output.
     *
     * Outputs the movie ID, title, release year, and rating.
     */
    void displayDetails() const;
};

#endif // MOVIE_H
