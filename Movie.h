#ifndef MOVIE_H
#define MOVIE_H

#include <string>
#include "Actor.h"
#include "List.h"

class Movie {
private:
    int id;
    std::string title;
    std::string plot;
    int releaseYear;
    List<Actor> actors; // List of Actor

public:
    Movie() : id(0), releaseYear(0) {}
    Movie(const std::string& title, const std::string& plot, int releaseYear, int id);

    int getId() const;
    void setId(int newId);

    std::string getTitle() const;
    void setTitle(const std::string& newTitle);

    std::string getPlot() const;
    void setPlot(const std::string& newPlot);

    int getReleaseYear() const;
    void setReleaseYear(int newYear);

    void addActor(const Actor& actor);
    bool hasActor(const std::string& actorName) const;

    // Access to the underlying actor list
    List<Actor>& getActors();
    const List<Actor>& getActors() const;

    void displayDetails() const;

    // For equality in List<Movie>
    bool operator==(const Movie& other) const {
        return (this->id == other.id);
    }

    // So that List<Movie>::display() can do std::cout << movie
    friend std::ostream& operator<<(std::ostream& os, const Movie& movie) {
        os << "[Movie ID=" << movie.id
            << ", Title=\"" << movie.title
            << "\", Year=" << movie.releaseYear
            << "]";
        return os;
    }
};

#endif // MOVIE_H
