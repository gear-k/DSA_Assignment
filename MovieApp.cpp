#include "MovieApp.h"
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

void MovieApp::addNewActor(const string& name, int yearOfBirth) {
    static int nextActorId = 1; // Static variable to generate unique IDs
    Actor newActor(name, yearOfBirth, nextActorId);
    actorList.add(newActor);
    cout << "New actor added: " << name << " with ID: " << nextActorId << endl;
    ++nextActorId; // Increment ID for the next actor
}

void MovieApp::addNewMovie(const string& title, const string& plot, int releaseYear) {
    static int nextMovieId = 1; // Static variable to generate unique IDs
    Movie newMovie(title, plot, releaseYear, nextMovieId);
    movieList.add(newMovie);
    cout << "New movie added: " << title << " with ID: " << nextMovieId << endl;
    ++nextMovieId; // Increment ID for the next movie
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

void MovieApp::readActors(const string& fileName) {
    ifstream file(fileName);
    string line;

    if (!file.is_open()) {
        cerr << "Failed to open " << fileName << endl;
        return;
    }

    // Skip the header line
    getline(file, line);

    while (getline(file, line)) {
        if (line.empty()) continue; // Skip empty lines

        stringstream ss(line);
        string idStr, name, birthStr;
        int id, yearOfBirth;

        // Parse ID
        if (!getline(ss, idStr, ',')) {
            cerr << "Malformed line (missing ID): " << line << endl;
            continue;
        }
        try {
            id = stoi(idStr); // Convert string to integer
        }
        catch (exception& e) {
            cerr << "Invalid ID format: " << idStr << endl;
            continue;
        }

        // Parse name
        if (!getline(ss, name, ',')) {
            cerr << "Malformed line (missing name): " << line << endl;
            continue;
        }

        // Parse birth year
        if (!getline(ss, birthStr)) {
            cerr << "Malformed line (missing birth year): " << line << endl;
            continue;
        }
        try {
            yearOfBirth = stoi(birthStr); // Convert string to integer
        }
        catch (exception& e) {
            cerr << "Invalid birth year format: " << birthStr << endl;
            continue;
        }

        // Add the actor using the ID
        Actor newActor(name, yearOfBirth, id); // Pass ID to the constructor
        actorList.add(newActor);
    }

    file.close();
    cout << "Actors loaded from " << fileName << endl;
}


void MovieApp::readMovies(const string& fileName) {
    ifstream file(fileName);
    string line;

    if (!file.is_open()) {
        cerr << "Failed to open " << fileName << endl;
        return;
    }

    // Skip the header line
    getline(file, line);

    while (getline(file, line)) {
        if (line.empty()) continue; // Skip empty lines

        stringstream ss(line);
        string idStr, title, yearStr;
        int id, releaseYear;

        // Parse ID
        if (!getline(ss, idStr, ',')) {
            cerr << "Malformed line (missing ID): " << line << endl;
            continue;
        }
        id = stoi(idStr);

        // Parse title
        if (!getline(ss, title, ',')) {
            cerr << "Malformed line (missing title): " << line << endl;
            continue;
        }

        // Parse release year
        if (!getline(ss, yearStr)) {
            cerr << "Malformed line (missing year): " << line << endl;
            continue;
        }
        releaseYear = stoi(yearStr);

        // Add the movie using the ID
        Movie newMovie(title, "", releaseYear, id);
        newMovie.setId(id); // Ensure your Movie class has an `id` attribute
        movieList.add(newMovie);
    }

    file.close();
    cout << "Movies loaded from " << fileName << endl;
}

void MovieApp::readCast(const string& fileName) {
    ifstream file(fileName);
    string line;

    if (!file.is_open()) {
        cerr << "Failed to open " << fileName << endl;
        return;
    }

    // Skip the header line
    getline(file, line);

    while (getline(file, line)) {
        if (line.empty()) continue; // Skip empty lines

        stringstream ss(line);
        string personIdStr, movieIdStr;
        int personId, movieId;

        // Parse person_id
        if (!getline(ss, personIdStr, ',')) {
            cerr << "Malformed line (missing person_id): " << line << endl;
            continue;
        }
        personId = stoi(personIdStr);

        // Parse movie_id
        if (!getline(ss, movieIdStr)) {
            cerr << "Malformed line (missing movie_id): " << line << endl;
            continue;
        }
        movieId = stoi(movieIdStr);

        // Find the actor and movie by ID
        Actor* actor = actorList.findById(personId);
        Movie* movie = movieList.findById(movieId);

        if (actor && movie) {
            movie->addActor(*actor);
            cout << "Actor " << actor->getName() << " added to movie " << movie->getTitle() << endl;
        }
        else {
            cerr << "Actor or movie not found for IDs: " << personId << ", " << movieId << endl;
        }
    }

    file.close();
    cout << "Cast loaded from " << fileName << endl;
}
