#ifndef MOVIE_H
#define MOVIE_H

#include "Actor.h"
#include "List.h"
#include <iostream>
#include <cstring>

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

class Movie {
private:
    int id;                  // Unique movie identifier
    char title[100];         // Title of the movie
    char plot[500];          // Short description of the movie's plot
    int releaseYear;         // Year the movie was released
    List<Actor> actors;      // List of actors in the movie
    int rating;              // Movie rating (0 to 10)

public:
    // Compare two movies based on their ID
    bool operator==(const Movie& other) const;

    // --------------------------
    // Constructors and Destructor
    // --------------------------

    // Default constructor initializes an empty movie with default values
    Movie();

    // Constructor to initialize a movie with title, plot, release year, and movie ID
    Movie(const char* ttl, const char* plt, int rYear, int mid);

    // Copy constructor to create a deep copy of another movie
    Movie(const Movie& other);

    // Assignment operator to copy one movie into another
    Movie& operator=(const Movie& other);

    // Default destructor
    ~Movie() = default;

    // --------------------------
    // Getters and Setters
    // --------------------------

    // Returns the movie's ID
    int getId() const;

    // Sets a new ID for the movie
    void setId(int newId);

    // Returns the movie's title
    const char* getTitle() const;

    // Updates the movie's title with a new string
    void setTitle(const char* newTitle);

    // Returns the movie's plot
    const char* getPlot() const;

    // Updates the movie's plot with a new string
    void setPlot(const char* newPlot);

    // Returns the release year of the movie
    int getReleaseYear() const;

    // Updates the release year of the movie
    void setReleaseYear(int newYear);

    // Returns the movie's rating
    int getRating() const { return rating; }

    // Sets a new rating for the movie (expected range: 0 to 10)
    void setRating(int r) { rating = r; }

    // --------------------------
    // Actor Management
    // --------------------------

    // Adds an actor to the movie's actor list if not already present
    void addActor(const Actor& actor);

    // Checks if an actor with the specified name is in the movie
    bool hasActor(const char* actorName) const;

    // Checks if an actor with the specified ID is in the movie
    bool hasActor(int actorId) const;

    // --------------------------
    // Access to Actors List
    // --------------------------

    // Returns a reference to the list of actors (modifiable version)
    List<Actor>& getActors();

    // Returns a reference to the list of actors (read-only version)
    const List<Actor>& getActors() const;

    // --------------------------
    // Display Methods
    // --------------------------

    // Displays movie details such as ID, title, release year, and rating
    void displayDetails() const;
};

#endif // MOVIE_H
