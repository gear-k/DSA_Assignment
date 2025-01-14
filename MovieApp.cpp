#include "MovieApp.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm> // For std::transform
#include <cctype>    // For std::tolower
#include <ctime>     // For getting the current year
using namespace std;

// Utility function to trim whitespace
void trimString(std::string& str) {
    while (!str.empty() && isspace(str.front())) {
        str.erase(0, 1);
    }
    while (!str.empty() && isspace(str.back())) {
        str.pop_back();
    }
}

// Utility function to clean CSV field data
std::string cleanCSVField(const std::string& field) {
    std::string cleaned = field;

    // Remove semicolons
    cleaned.erase(std::remove(cleaned.begin(), cleaned.end(), ';'), cleaned.end());

    // Remove quotes if they exist at start and end
    if (cleaned.size() >= 2 && cleaned.front() == '"' && cleaned.back() == '"') {
        cleaned = cleaned.substr(1, cleaned.size() - 2);
    }

    // Remove any \r characters (carriage returns)
    cleaned.erase(std::remove(cleaned.begin(), cleaned.end(), '\r'), cleaned.end());

    // Trim whitespace
    trimString(cleaned);

    return cleaned;
}

// Utility function to convert string to lowercase
std::string toLowerCase(const std::string& str) {
    std::string lowerStr = str;
    std::transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(),
        [](unsigned char c) { return std::tolower(c); });
    return lowerStr;
}

// Add New Actor
void MovieApp::addNewActor(const std::string& name, int yearOfBirth) {
    static int nextActorId = 1;  // Ensure unique IDs
    Actor newActor(name, yearOfBirth, nextActorId++);
    actorList.add(newActor);
    cout << "Actor added successfully!\n";
}

// Add New Movie
void MovieApp::addNewMovie(const std::string& title, const std::string& plot, int releaseYear) {
    static int nextMovieId = 1;  // Ensure unique IDs
    Movie newMovie(title, plot, releaseYear, nextMovieId++);
    movieList.add(newMovie);
    cout << "Movie added successfully!\n";
}

// Add Actor to Movie
void MovieApp::addActorToMovie(const std::string& actorName, const std::string& movieTitle) {
    std::string trimmedActorName = actorName, trimmedMovieTitle = movieTitle;
    trimString(trimmedActorName);
    trimString(trimmedMovieTitle);
    trimmedActorName = toLowerCase(trimmedActorName);
    trimmedMovieTitle = toLowerCase(trimmedMovieTitle);

    Actor* actor = nullptr;
    for (int i = 1; i <= actorList.getLength(); ++i) {
        if (toLowerCase(actorList.get(i)->getName()) == trimmedActorName) {
            actor = actorList.get(i);
            break;
        }
    }

    Movie* movie = nullptr;
    for (int i = 1; i <= movieList.getLength(); ++i) {
        if (toLowerCase(movieList.get(i)->getTitle()) == trimmedMovieTitle) {
            movie = movieList.get(i);
            break;
        }
    }

    if (actor && movie) {
        movie->addActor(*actor);
        cout << "Actor added to movie successfully!\n";
    }
    else {
        cout << "Error: Actor or Movie not found!\n";
    }
}

// Update Actor Details
void MovieApp::updateActorDetails(int id, const std::string& newName, int newYearOfBirth) {
    Actor* actor = actorList.findById(id);
    if (actor) {
        actor->setName(newName);
        actor->setYearOfBirth(newYearOfBirth);
        cout << "Actor details updated successfully!\n";
    }
    else {
        cout << "Error: Actor ID not found.\n";
    }
}

// Update Movie Details
void MovieApp::updateMovieDetails(int id, const std::string& newTitle, const std::string& newPlot, int newReleaseYear) {
    Movie* movie = movieList.findById(id);
    if (movie) {
        movie->setTitle(newTitle);
        movie->setPlot(newPlot);
        movie->setReleaseYear(newReleaseYear);
        cout << "Movie details updated successfully!\n";
    }
    else {
        cout << "Error: Movie ID not found.\n";
    }
}

// Modified readActors function
void MovieApp::readActors(const string& fileName) {
    ifstream file(fileName);
    if (!file.is_open()) {
        cerr << "Error opening file: " << fileName << endl;
        return;
    }

    string line;
    getline(file, line);  // Skip header
    while (getline(file, line)) {
        stringstream ss(line);
        string idStr, name, birthStr;
        int id, yearOfBirth;

        getline(ss, idStr, ',');
        getline(ss, name, ',');
        getline(ss, birthStr);

        // Clean the fields
        idStr = cleanCSVField(idStr);
        name = cleanCSVField(name);
        birthStr = cleanCSVField(birthStr);

        try {
            id = stoi(idStr);
            yearOfBirth = stoi(birthStr);

            actorList.add(Actor(name, yearOfBirth, id));
        }
        catch (const std::exception& e) {
            cerr << "Error processing line: " << line << "\nError: " << e.what() << endl;
            continue;
        }
    }
    file.close();
}

// Modified readMovies function
void MovieApp::readMovies(const string& fileName) {
    ifstream file(fileName);
    if (!file.is_open()) {
        cerr << "Error opening file: " << fileName << endl;
        return;
    }

    string line;
    getline(file, line);  // Skip header
    while (getline(file, line)) {
        stringstream ss(line);
        string idStr, title, yearStr;
        int id, releaseYear;

        getline(ss, idStr, ',');
        getline(ss, title, ',');
        getline(ss, yearStr);

        // Clean the fields
        idStr = cleanCSVField(idStr);
        title = cleanCSVField(title);
        yearStr = cleanCSVField(yearStr);

        try {
            id = stoi(idStr);
            releaseYear = stoi(yearStr);

            movieList.add(Movie(title, "", releaseYear, id));
        }
        catch (const std::exception& e) {
            cerr << "Error processing line: " << line << "\nError: " << e.what() << endl;
            continue;
        }
    }
    file.close();
}

// Modified readCast function
void MovieApp::readCast(const string& fileName) {
    ifstream file(fileName);
    if (!file.is_open()) {
        cerr << "Error opening file: " << fileName << endl;
        return;
    }

    string line;
    getline(file, line);  // Skip header
    while (getline(file, line)) {
        stringstream ss(line);
        string actorIdStr, movieIdStr;
        int actorId, movieId;

        getline(ss, actorIdStr, ',');
        getline(ss, movieIdStr);

        // Clean the fields
        actorIdStr = cleanCSVField(actorIdStr);
        movieIdStr = cleanCSVField(movieIdStr);

        try {
            actorId = stoi(actorIdStr);
            movieId = stoi(movieIdStr);

            Actor* actor = actorList.findById(actorId);
            Movie* movie = movieList.findById(movieId);

            if (actor && movie) {
                movie->addActor(*actor);
            }
        }
        catch (const std::exception& e) {
            cerr << "Error processing line: " << line << "\nError: " << e.what() << endl;
            continue;
        }
    }
    file.close();
}

// Display All Actors
void MovieApp::displayAllActors() const {
    actorList.displayAll();
}

// Display All Movies
void MovieApp::displayAllMovies() const {
    movieList.displayAll();
}

// Display Movies of an Actor
void MovieApp::displayMoviesOfActor(const std::string& actorName) const {
    std::string trimmedActorName = actorName;
    trimString(trimmedActorName);
    trimmedActorName = toLowerCase(trimmedActorName);

    const Actor* actor = nullptr;
    for (int i = 1; i <= actorList.getLength(); i++) {
        const Actor* temp = actorList.get(i);
        if (toLowerCase(temp->getName()) == trimmedActorName) {
            actor = temp;
            break;
        }
    }

    if (!actor) {
        std::cout << "Error: Actor \"" << actorName << "\" not found.\n";
        return;
    }

    std::cout << "\nMovies starring " << actorName << ":\n";
    bool found = false;
    for (int i = 1; i <= movieList.getLength(); i++) {
        const Movie* movie = movieList.get(i);
        if (!movie) continue;
        const ActorList& cast = movie->getActors();
        for (int j = 1; j <= cast.getLength(); j++) {
            const Actor* castActor = cast.get(j);
            if (castActor->getId() == actor->getId()) {
                std::cout << movie->getTitle() << std::endl;
                found = true;
                break;
            }
        }
    }

    if (!found) {
        std::cout << "No movies found for actor \"" << actorName << "\".\n";
    }
}

// Display Actors in a Movie
void MovieApp::displayActorsInMovie(const std::string& movieTitle) const {
    std::string trimmedMovieTitle = movieTitle;
    trimString(trimmedMovieTitle);
    trimmedMovieTitle = toLowerCase(trimmedMovieTitle);

    const Movie* movie = nullptr;
    for (int i = 1; i <= movieList.getLength(); ++i) {
        const Movie* temp = movieList.get(i);
        if (toLowerCase(temp->getTitle()) == trimmedMovieTitle) {
            movie = temp;
            break;
        }
    }

    if (!movie) {
        std::cout << "Error: Movie \"" << movieTitle << "\" not found.\n";
        return;
    }

    std::cout << "\nActors in \"" << movieTitle << "\":\n";
    if (movie->getActors().getLength() == 0) {
        std::cout << "No actors found in the movie \"" << movieTitle << "\".\n";
    }
    else {
        movie->getActors().displayAll();
    }
}

// Display Actors Known by an Actor
void MovieApp::displayActorsKnownBy(const std::string& actorName) const {
    std::string trimmedActorName = actorName;
    trimString(trimmedActorName);
    trimmedActorName = toLowerCase(trimmedActorName);

    const Actor* actor = nullptr;
    for (int i = 1; i <= actorList.getLength(); ++i) {
        const Actor* temp = actorList.get(i);
        if (toLowerCase(temp->getName()) == trimmedActorName) {
            actor = temp;
            break;
        }
    }

    if (!actor) {
        std::cout << "Error: Actor \"" << actorName << "\" not found.\n";
        return;
    }

    ActorList knownActors;

    for (int i = 1; i <= movieList.getLength(); ++i) {
        const Movie* movie = movieList.get(i);
        if (!movie) continue;

        bool inMovie = false;
        const ActorList& cast = movie->getActors();
        for (int j = 1; j <= cast.getLength(); ++j) {
            const Actor* castActor = cast.get(j);
            if (castActor->getId() == actor->getId()) {
                inMovie = true;
                break;
            }
        }

        if (inMovie) {
            for (int j = 1; j <= cast.getLength(); ++j) {
                const Actor* coActor = cast.get(j);
                if (coActor && coActor->getId() != actor->getId() &&
                    !knownActors.findById(coActor->getId())) {
                    knownActors.add(*coActor);
                }
            }
        }
    }

    std::cout << "\nActors known by " << actorName << ":\n";
    knownActors.displayAll();
}

// Display Actors by Age Range
void MovieApp::displayActorsByAge(int minAge, int maxAge) const {
    cout << "Actors between age " << minAge << " and " << maxAge << ":\n";
    bool found = false;
    for (int i = 1; i <= actorList.getLength(); ++i) {
        const Actor* actor = actorList.get(i);
        if (actor && actor->getAge() >= minAge && actor->getAge() <= maxAge) {
            actor->displayDetails();
            found = true;
        }
    }
    if (!found) {
        cout << "No actors found in the specified age range.\n";
    }
}

// Display Recent Movies (last 3 years)
void MovieApp::displayRecentMovies() const {
    time_t now = time(0);
    tm localTimeStruct;
    localtime_s(&localTimeStruct, &now);
    int currentYear = 1900 + localTimeStruct.tm_year;
    int recentThreshold = currentYear - 3;

    cout << "Movies released in the last 3 years:\n";
    bool found = false;
    for (int i = 1; i <= movieList.getLength(); ++i) {
        const Movie* movie = movieList.get(i);
        if (movie && movie->getReleaseYear() >= recentThreshold) {
            movie->displayDetails();
            found = true;
        }
    }
    if (!found) {
        cout << "No recent movies found.\n";
    }
}
