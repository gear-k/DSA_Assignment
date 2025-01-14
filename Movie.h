#ifndef MOVIE_H
#define MOVIE_H

#include <string>
#include "ActorList.h"

class Movie {
private:
    int id;           // Unique ID for the movie
    std::string title;
    std::string plot;
    int releaseYear;
    ActorList actors; // List of actors in the movie

public:
    Movie(const std::string& title, const std::string& plot, int releaseYear, int id);

    int getId() const;
    std::string getTitle() const;
    void setTitle(const std::string& newTitle);
    void setPlot(const std::string& newPlot);
    void setReleaseYear(int newYear);

    ActorList& getActors();                 // Non-const version
    const ActorList& getActors() const;      // Const version

    int getReleaseYear() const;
    void addActor(const Actor& actor);
    void displayDetails() const;
};

#endif
