#include "MovieApp.h"
#include <iostream>
using namespace std;

void MovieApp::addNewActor(const string& name, int yearOfBirth) {
    Actor newActor(name, yearOfBirth);
    actorList.add(newActor);
    cout << "New actor added: " << name << endl;
}

void MovieApp::addNewMovie(const string& title, const string& plot, int releaseYear) {
    Movie newMovie(title, plot, releaseYear);
    movieList.add(newMovie);
    cout << "New movie added: " << title << endl;
}

void MovieApp::addActorToMovie(const string& actorName, const string& movieTitle) {
    Actor* actor = nullptr;
    for (int i = 1; i <= actorList.getLength(); ++i) {
        if (actorList.get(i)->getName() == actorName) {
            actor = actorList.get(i);
            break;
        }
    }

    Movie* movie = nullptr;
    for (int i = 1; i <= movieList.getLength(); ++i) {
        if (movieList.get(i)->getTitle() == movieTitle) {
            movie = movieList.get(i);
            break;
        }
    }

    if (actor && movie) {
        movie->addActor(*actor);
        cout << "Actor added to movie!" << endl;
    }
    else {
        cout << "Actor or movie not found!" << endl;
    }
}

void MovieApp::displayAllActors() const {
    cout << "All Actors:" << endl;
    actorList.displayAll();
}

void MovieApp::displayAllMovies() const {
    cout << "All Movies:" << endl;
    movieList.displayAll();
}
