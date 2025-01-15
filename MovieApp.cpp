#include "MovieApp.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
using namespace std;

MovieApp::MovieApp()
    : nextActorId(1000), // or pick any starting ID
    nextMovieId(5000)
{
}

//------------------------------------------------------------------------------
// 1) CSV Reading
//------------------------------------------------------------------------------

// Trims leading and trailing whitespace and quotes
std::string trimQuotes(const std::string& str) {
    size_t start = str.find_first_not_of(" \"");
    size_t end = str.find_last_not_of(" \"");

    if (start == std::string::npos || end == std::string::npos) {
        return ""; // Return empty string if all characters are whitespace or quotes
    }
    return str.substr(start, end - start + 1);
}


void MovieApp::readActors(const std::string& filename) {
    std::ifstream fin(filename);
    if (!fin.is_open()) {
        std::cerr << "Error: Could not open file \"" << filename << "\"\n";
        return;
    }

    std::string header;
    getline(fin, header); // Skip header line

    std::string line;
    while (getline(fin, line)) {
        if (line.empty()) continue;

        std::stringstream ss(line);
        std::string idStr, name, birthStr;

        getline(ss, idStr, ',');
        getline(ss, name, ',');
        getline(ss, birthStr, ',');

        int id = std::stoi(idStr);
        int birth = std::stoi(birthStr);

        // Trim quotes from the name
        name = trimQuotes(name);

        // Add the actor
        Actor actor(name, birth, id);
        actorList.add(actor);

        // Update the next available ID
        if (id >= nextActorId) {
            nextActorId = id + 1;
        }
    }

    fin.close();
}


void MovieApp::readMovies(const std::string& filename) {
    std::ifstream fin(filename);
    if (!fin.is_open()) {
        std::cerr << "Error: Could not open file \"" << filename << "\"\n";
        return;
    }

    std::string header;
    getline(fin, header); // Skip header line

    std::string line;
    while (getline(fin, line)) {
        if (line.empty()) continue;

        std::stringstream ss(line);
        std::string idStr, title, yearStr;

        getline(ss, idStr, ',');
        getline(ss, title, ',');
        getline(ss, yearStr, ',');

        int id = std::stoi(idStr);
        int year = std::stoi(yearStr);

        // Trim quotes from the title
        title = trimQuotes(title);

        // Add the movie
        Movie movie(title, "", year, id); // Plot is empty in the CSV
        movieList.add(movie);

        // Update the next available ID
        if (id >= nextMovieId) {
            nextMovieId = id + 1;
        }
    }

    fin.close();
}

void MovieApp::readCast(const std::string& filename) {
    // This CSV: person_id,movie_id
    // We want to find the actor with that person_id, the movie with movie_id, and associate them.
    ifstream fin(filename);
    if (!fin.is_open()) {
        cerr << "Error: Could not open file \"" << filename << "\"\n";
        return;
    }
    // Skip header line: "person_id,movie_id"
    string header;
    getline(fin, header);

    string line;
    while (getline(fin, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        string personIdStr, movieIdStr;
        getline(ss, personIdStr, ',');
        getline(ss, movieIdStr, ',');

        if (personIdStr.empty() || movieIdStr.empty()) {
            continue;
        }
        int personId = stoi(personIdStr);
        int movieId = stoi(movieIdStr);

        // Find the actor object by ID
        Actor* foundActor = nullptr;
        actorList.display([&](Actor& a) {
            if (a.getId() == personId) {
                foundActor = &a;
                return true; // break
            }
            return false;
            });

        // Find the movie object by ID
        Movie* foundMovie = nullptr;
        movieList.display([&](Movie& m) {
            if (m.getId() == movieId) {
                foundMovie = &m;
                return true;
            }
            return false;
            });

        if (foundActor && foundMovie) {
            foundMovie->addActor(*foundActor);
        }
    }
    fin.close();
}

//------------------------------------------------------------------------------
// 2) "Add New" items
//------------------------------------------------------------------------------

void MovieApp::addNewActor(const std::string& name, int birthYear) {
    Actor actor(name, birthYear, nextActorId);
    ++nextActorId;
    actorList.add(actor);
    cout << "Added new actor: \"" << name << "\" (ID=" << actor.getId() << ")\n";
}

void MovieApp::addNewMovie(const std::string& title, const std::string& plot, int releaseYear) {
    Movie movie(title, plot, releaseYear, nextMovieId);
    ++nextMovieId;
    movieList.add(movie);
    cout << "Added new movie: \"" << title << "\" (ID=" << movie.getId() << ")\n";
}

//------------------------------------------------------------------------------
// 3) Link actors & movies
//------------------------------------------------------------------------------

void MovieApp::addActorToMovie(const std::string& actorName, const std::string& movieTitle) {
    Movie* targetMovie = nullptr;
    movieList.display([&](Movie& m) {
        if (m.getTitle() == movieTitle) {
            targetMovie = &m;
            return true; // break
        }
        return false;
        });

    if (!targetMovie) {
        cout << "[Error] Movie \"" << movieTitle << "\" not found.\n";
        return;
    }

    Actor* targetActor = nullptr;
    actorList.display([&](Actor& a) {
        if (a.getName() == actorName) {
            targetActor = &a;
            return true; // break
        }
        return false;
        });

    if (!targetActor) {
        cout << "[Error] Actor \"" << actorName << "\" not found.\n";
        return;
    }

    // Add the Actor to the Movie
    targetMovie->addActor(*targetActor);
    cout << "Actor \"" << actorName << "\" added to movie \"" << movieTitle << "\"\n";
}

//------------------------------------------------------------------------------
// 4) Update actor/movie
//------------------------------------------------------------------------------

void MovieApp::updateActorDetails(int actorId, const std::string& newName, int newYearOfBirth) {
    bool found = false;
    actorList.display([&](Actor& a) {
        if (a.getId() == actorId) {
            a.setName(newName);
            a.setBirthYear(newYearOfBirth);
            cout << "Updated Actor ID " << actorId
                << " => Name: " << newName
                << ", BirthYear: " << newYearOfBirth << endl;
            found = true;
            return true; // break
        }
        return false;
        });
    if (!found) {
        cout << "[Error] Actor ID " << actorId << " not found.\n";
    }
}

void MovieApp::updateMovieDetails(int movieId, const std::string& newTitle, const std::string& newPlot, int newReleaseYear) {
    bool found = false;
    movieList.display([&](Movie& m) {
        if (m.getId() == movieId) {
            m.setTitle(newTitle);
            m.setPlot(newPlot);
            m.setReleaseYear(newReleaseYear);
            cout << "Updated Movie ID " << movieId
                << " => Title: " << newTitle
                << ", Plot: " << newPlot
                << ", Year: " << newReleaseYear << endl;
            found = true;
            return true; // break
        }
        return false;
        });
    if (!found) {
        cout << "[Error] Movie ID " << movieId << " not found.\n";
    }
}

//------------------------------------------------------------------------------
// 5) Display
//------------------------------------------------------------------------------

void MovieApp::displayAllActors() const {
    if (actorList.isEmpty()) {
        std::cout << "No actors found.\n";
        return;
    }
    actorList.display([](const Actor& actor) {
        actor.displayDetails();
        return false; // Continue displaying
        });
}


void MovieApp::displayAllMovies() const {
    if (movieList.isEmpty()) {
        std::cout << "No movies found.\n";
        return;
    }
    movieList.display([](const Movie& movie) {
        movie.displayDetails();
        return false; // Continue displaying
        });
}


void MovieApp::displayActorsByAge(int minAge, int maxAge) const {
    bool anyFound = false;
    actorList.display([&](const Actor& a) {
        int age = a.getAge();
        if (age >= minAge && age <= maxAge) {
            cout << a.getName() << " (Age=" << age << ")\n";
            anyFound = true;
        }
        return false;
        });
    if (!anyFound) {
        cout << "No actors found in age range [" << minAge << ", " << maxAge << "].\n";
    }
}

void MovieApp::displayRecentMovies() const {
    // "last 3 years" means releaseYear >= 2025 - 3 => 2022
    const int cutoff = 2025 - 3;
    bool anyFound = false;
    movieList.display([&](const Movie& m) {
        if (m.getReleaseYear() >= cutoff) {
            cout << m.getTitle() << " (" << m.getReleaseYear() << ")\n";
            anyFound = true;
        }
        return false;
        });
    if (!anyFound) {
        cout << "No movies found released in the last 3 years.\n";
    }
}

void MovieApp::displayMoviesOfActor(const std::string& actorName) const {
    bool anyFound = false;
    movieList.display([&](const Movie& m) {
        if (m.hasActor(actorName)) {
            cout << m.getTitle() << " (" << m.getReleaseYear() << ")\n";
            anyFound = true;
        }
        return false;
        });
    if (!anyFound) {
        cout << "No movies found for actor \"" << actorName << "\".\n";
    }
}

void MovieApp::displayActorsInMovie(const std::string& movieTitle) const {
    bool foundMovie = false;
    movieList.display([&](const Movie& m) {
        if (m.getTitle() == movieTitle) {
            foundMovie = true;
            cout << "Actors in \"" << movieTitle << "\":\n";
            // Just display them
            m.getActors().display();
            return true; // break
        }
        return false;
        });
    if (!foundMovie) {
        cout << "Movie \"" << movieTitle << "\" not found.\n";
    }
}

void MovieApp::displayActorsKnownBy(const std::string& actorName) const {
    // We'll gather all distinct actors that share a movie with `actorName`
    List<std::string> knownNames; // store actor names

    // For each movie, if it has `actorName`, add other cast members
    movieList.display([&](const Movie& m) {
        if (m.hasActor(actorName)) {
            // Add all other actors
            m.getActors().display([&](const Actor& a) {
                if (a.getName() != actorName) {
                    // Check if already in knownNames
                    bool alreadyExists = false;
                    knownNames.display([&](const std::string& existing) {
                        if (existing == a.getName()) {
                            alreadyExists = true;
                            return true; // break
                        }
                        return false;
                        });
                    if (!alreadyExists) {
                        // Add and also print
                        knownNames.add(a.getName());
                        cout << a.getName() << endl;
                    }
                }
                return false;
                });
        }
        return false;
        });
}
