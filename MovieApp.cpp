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

#include "MovieApp.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm> // For trimming and case handling
using namespace std;

// Utility function to trim whitespace and quotes from a string
void trimString(string& str) {
    // Remove leading whitespace
    while (!str.empty() && isspace(str.front())) {
        str.erase(0, 1);
    }
    // Remove trailing whitespace
    while (!str.empty() && isspace(str.back())) {
        str.pop_back();
    }
    // Remove surrounding quotes
    if (!str.empty() && str.front() == '"') {
        str.erase(0, 1);
    }
    if (!str.empty() && str.back() == '"') {
        str.pop_back();
    }
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
        trimString(name); // Remove quotes and whitespace

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
        try {
            id = stoi(idStr);
        }
        catch (exception& e) {
            cerr << "Invalid ID format: " << idStr << endl;
            continue;
        }

        // Parse title
        if (!getline(ss, title, ',')) {
            cerr << "Malformed line (missing title): " << line << endl;
            continue;
        }
        trimString(title); // Remove quotes and whitespace

        // Parse release year
        if (!getline(ss, yearStr)) {
            cerr << "Malformed line (missing year): " << line << endl;
            continue;
        }
        try {
            releaseYear = stoi(yearStr);
        }
        catch (exception& e) {
            cerr << "Invalid release year format: " << yearStr << endl;
            continue;
        }

        // Add the movie using the ID
        Movie newMovie(title, "", releaseYear, id);
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
        try {
            personId = stoi(personIdStr);
        }
        catch (exception& e) {
            cerr << "Invalid person ID format: " << personIdStr << endl;
            continue;
        }

        // Parse movie_id
        if (!getline(ss, movieIdStr)) {
            cerr << "Malformed line (missing movie_id): " << line << endl;
            continue;
        }
        try {
            movieId = stoi(movieIdStr);
        }
        catch (exception& e) {
            cerr << "Invalid movie ID format: " << movieIdStr << endl;
            continue;
        }

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

// (g)
void MovieApp::displayMoviesOfActor(const string& actorName) const {
    // find the actor
    const Actor* foundActor = nullptr;
    for (int i = 1; i <= actorList.getLength(); i++) {
        const Actor* temp = actorList.get(i); // calls const version
        if (temp && temp->getName() == actorName) {
            foundActor = temp;
            break;
        }
    }
    if (!foundActor) {
        cout << "Actor \"" << actorName << "\" not found.\n";
        return;
    }

    // create a temporary MovieList
    MovieList actorMovies;

    // loop through all movies
    for (int i = 1; i <= movieList.getLength(); i++) {
        const Movie* m = movieList.get(i); // calls const version
        if (!m) continue;

        // cast is also const
        const ActorList& cast = m->getActors();
        for (int j = 1; j <= cast.getLength(); j++) {
            const Actor* a = cast.get(j); // calls const version
            if (a && a->getId() == foundActor->getId()) {
                // Even though m is const, actorMovies.add() stores a *copy* 
                // so we can safely pass *m
                actorMovies.add(*m);
                break;
            }
        }
    }

    actorMovies.sortByTitle();
    cout << "\nMovies starring " << actorName << ":\n";
    actorMovies.displayAll();
    cout << endl;
}

// (h)
void MovieApp::displayActorsInMovie(const string& movieTitle) const {
    const Movie* foundMovie = nullptr;
    for (int i = 1; i <= movieList.getLength(); i++) {
        const Movie* temp = movieList.get(i);
        if (temp) {
            cout << "Checking movie: [" << temp->getTitle() << "]" << endl; // Debug
            if (temp->getTitle() == movieTitle) {
                foundMovie = temp;
                break;
            }
        }
    }

    if (!foundMovie) {
        cout << "Movie \"" << movieTitle << "\" not found.\n";
        return;
    }

    ActorList sortedActors;
    const ActorList& original = foundMovie->getActors();
    for (int i = 1; i <= original.getLength(); i++) {
        const Actor* a = original.get(i);
        if (a) {
            // store a copy
            sortedActors.add(*a);
        }
    }
    sortedActors.sortByName();

    cout << "\nActors in \"" << movieTitle << "\":\n";
    sortedActors.displayAll();
    cout << endl;
}

// (i)
void MovieApp::displayActorsKnownBy(const string& actorName) const {
    const Actor* foundActor = nullptr;

    // Search for the actor in the list
    for (int i = 1; i <= actorList.getLength(); i++) {
        const Actor* temp = actorList.get(i); // Safely retrieve actor
        if (temp && temp->getName() == actorName) {
            foundActor = temp;
            break;
        }
    }

    if (!foundActor) {
        cout << "Actor \"" << actorName << "\" not found.\n";
        return;
    }

    // Proceed only if foundActor is valid
    cout << "Found actor: " << foundActor->getName() << endl;

    ActorList knownActors;

    // traverse every movie
    for (int i = 1; i <= movieList.getLength(); i++) {
        const Movie* m = movieList.get(i);
        if (!m) continue;

        bool actorIsInMovie = false;
        const ActorList& cast = m->getActors();
        for (int j = 1; j <= cast.getLength(); j++) {
            const Actor* a = cast.get(j);
            if (a && a->getId() == foundActor->getId()) {
                actorIsInMovie = true;
                break;
            }
        }

        if (actorIsInMovie) {
            for (int j = 1; j <= cast.getLength(); j++) {
                const Actor* coStar = cast.get(j);
                if (coStar && coStar->getId() != foundActor->getId()) {
                    if (!knownActors.containsId(coStar->getId())) {
                        knownActors.add(*coStar);
                    }
                }
            }
        }
    }

    knownActors.sortByName();
    cout << "\nActors known by " << actorName << " (co-stars):\n";
    knownActors.displayAll();
    cout << endl;
}
