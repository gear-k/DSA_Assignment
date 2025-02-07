// MovieApp.cpp
#include "MovieApp.h"
#include "ActorGraph.h"            // If you still use ActorGraph utilities
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <cstdlib>   // atoi
#include <ctime>     // for date-based logic (e.g., recent movies)
#include <cassert> // for debugging purposes

// Helper to trim quotes/spaces
static std::string trimQuotes(const std::string& str) {
    size_t start = str.find_first_not_of(" \"");
    size_t end = str.find_last_not_of(" \"");
    if (start == std::string::npos || end == std::string::npos) {
        return "";
    }
    return str.substr(start, end - start + 1);
}

// Helper to trim leading/trailing spaces
static std::string trim(const std::string& str) {
    size_t start = str.find_first_not_of(" \t");
    size_t end = str.find_last_not_of(" \t");
    if (start == std::string::npos || end == std::string::npos) {
        return "";
    }
    return str.substr(start, end - start + 1);
}

// Constructor
MovieApp::MovieApp()
    : actorTable(2000),
    movieTable(2000),
    nextActorId(1000),
    nextMovieId(5000),
    isAdmin(false)
{
    // Nothing else needed
}

//---------------------------------------------------------------------------
// Admin Mode
//---------------------------------------------------------------------------
void MovieApp::setAdminMode(bool admin) {
    isAdmin = admin;
    if (isAdmin) {
        std::cout << "[Mode] Administrator mode enabled.\n";
    }
    else {
        std::cout << "[Mode] User mode enabled.\n";
    }
}

bool MovieApp::isAdminMode() const {
    return isAdmin;
}

//---------------------------------------------------------------------------
// CSV Reading
//---------------------------------------------------------------------------
void MovieApp::readActors(const std::string& filename) {
    std::ifstream fin(filename.c_str());
    if (!fin.is_open()) {
        //std::cerr << "[Error] Could not open file: " << filename << "\n";
        return;
    }

    std::string header;
    if (!std::getline(fin, header)) {
        //std::cerr << "[Error] File is empty: " << filename << "\n";
        fin.close();
        return;
    }

    std::string line;
    while (std::getline(fin, line)) {
        if (line.empty()) continue;

        std::stringstream ss(line);
        std::string idStr, nameStr, birthStr;

        // CSV assumed format: actorId,name,birthYear
        std::getline(ss, idStr, ',');
        std::getline(ss, nameStr, ',');
        std::getline(ss, birthStr, ',');

        if (idStr.empty() || nameStr.empty() || birthStr.empty()) {
            /*std::cerr << "[Warning] Malformed row: " << line << "\n";*/
            continue;
        }

        int id = std::atoi(idStr.c_str());
        int birth = std::atoi(birthStr.c_str());
        nameStr = trimQuotes(nameStr);

        Actor actor(nameStr.c_str(), birth, id);
        actorTable.insert(actor);

        // Keep nextActorId higher than any existing ID
        if (id >= nextActorId) {
            nextActorId = id + 1;
        }
    }
    fin.close();
}

void MovieApp::readMovies(const std::string& filename) {
    std::ifstream fin(filename.c_str());
    if (!fin.is_open()) {
        std::cerr << "[Error] Could not open file: " << filename << "\n";
        return;
    }

    // Read (and optionally ignore) the header line
    std::string header;
    if (!std::getline(fin, header)) {
        std::cerr << "[Error] File is empty: " << filename << "\n";
        fin.close();
        return;
    }

    std::string line;
    while (std::getline(fin, line)) {
        if (line.empty()) continue;

        std::stringstream ss(line);

        // We now expect 4 comma-separated fields: id, title, plot, year
        std::string idStr, titleStr, plotStr, yearStr;

        std::getline(ss, idStr, ',');
        std::getline(ss, titleStr, ',');
        std::getline(ss, plotStr, ',');
        std::getline(ss, yearStr, ',');

        // Basic validation
        if (idStr.empty() || titleStr.empty() || plotStr.empty() || yearStr.empty()) {
            std::cerr << "[Warning] Malformed row: " << line << "\n";
            continue;
        }

        int id = std::atoi(idStr.c_str());
        int year = std::atoi(yearStr.c_str());

        // If you have a trimQuotes function to remove surrounding quotes
        titleStr = trimQuotes(titleStr);
        plotStr = trimQuotes(plotStr);

        // Pass the plot into the Movie constructor
        Movie movie(titleStr.c_str(), plotStr.c_str(), year, id);

        movieTable.insert(movie);

        if (id >= nextMovieId) {
            nextMovieId = id + 1;
        }
    }
    fin.close();
}


void MovieApp::readCast(const std::string& filename) {
    std::ifstream fin(filename.c_str());
    if (!fin.is_open()) {
        std::cerr << "[Error] Could not open file: " << filename << "\n";
        return;
    }

    std::string header;
    if (!std::getline(fin, header)) {
        std::cerr << "[Error] File is empty: " << filename << "\n";
        fin.close();
        return;
    }

    std::string line;
    while (std::getline(fin, line)) {
        if (line.empty()) continue;

        std::stringstream ss(line);
        std::string actorIdStr, movieIdStr;

        // CSV assumed format: actorId,movieId
        std::getline(ss, actorIdStr, ',');
        std::getline(ss, movieIdStr, ',');

        if (actorIdStr.empty() || movieIdStr.empty()) {
            //std::cerr << "[Warning] Malformed row: " << line << "\n";
            continue;
        }

        int aId = std::atoi(actorIdStr.c_str());
        int mId = std::atoi(movieIdStr.c_str());

        // Find them in the hash tables
        Actor* actor = actorTable.find(aId);
        Movie* movie = movieTable.find(mId);

        if (actor && movie) {
            movie->addActor(*actor);
        }
        else {
            //std::cerr << "[Warning] Invalid actorId or movieId in cast: " << line << "\n";
        }
    }
    fin.close();
}

//---------------------------------------------------------------------------
// Next ID accessors
//---------------------------------------------------------------------------
int MovieApp::getNextActorId() const {
    return nextActorId;
}

int MovieApp::getNextMovieId() const {
    return nextMovieId;
}

//---------------------------------------------------------------------------
// ID checks
//---------------------------------------------------------------------------
bool MovieApp::isActorIdUsed(int id) const {
    return (actorTable.find(id) != nullptr);
}

bool MovieApp::isMovieIdUsed(int id) const {
    return (movieTable.find(id) != nullptr);
}

//---------------------------------------------------------------------------
// Add new Actor
//---------------------------------------------------------------------------
void MovieApp::addNewActor(const std::string& name, int birthYear) {
    if (!isAdmin) {
        std::cout << "[Error] Only administrators can add new actors.\n";
        return;
    }
    std::string trimmedName = trim(name);
    if (trimmedName.empty()) {
        std::cout << "[Error] Actor name cannot be empty.\n";
        return;
    }
    if (birthYear <= 0 || birthYear >= 2025) {
        std::cout << "[Error] Invalid birth year.\n";
        return;
    }

    // Ensure unique ID
    while (isActorIdUsed(nextActorId)) {
        ++nextActorId;
    }

    Actor actor(trimmedName.c_str(), birthYear, nextActorId);
    actorTable.insert(actor);
    std::cout << "[Success] Added new actor: \"" << trimmedName
        << "\" (ID=" << actor.getId() << ")\n";
    ++nextActorId;
}

//---------------------------------------------------------------------------
// Add new Movie
//---------------------------------------------------------------------------
void MovieApp::addNewMovie(const std::string& title, const std::string& plot, int releaseYear) {
    if (!isAdmin) {
        std::cout << "[Error] Only administrators can add new movies.\n";
        return;
    }
    std::string trimmedTitle = trimQuotes(title); // Use trimQuotes instead of trim
    if (trimmedTitle.empty()) {
        std::cout << "[Error] Movie title cannot be empty.\n";
        return;
    }
    if (releaseYear <= 1800 || releaseYear >= 2025) {
        std::cout << "[Error] Invalid release year.\n";
        return;
    }

    while (isMovieIdUsed(nextMovieId)) {
        ++nextMovieId;
    }

    Movie movie(trimmedTitle.c_str(), plot.c_str(), releaseYear, nextMovieId);
    movieTable.insert(movie);
    std::cout << "[Success] Added new movie: \"" << trimmedTitle
        << "\" (ID=" << movie.getId() << ")\n";
    ++nextMovieId;
}



//---------------------------------------------------------------------------
// Link actor to movie
//---------------------------------------------------------------------------
void MovieApp::addActorToMovieById(int actorId, int movieId, bool isAdmin) {
    if (!isAdmin) {
        std::cout << "[Error] Only administrators can add actors to movies.\n";
        return;
    }

    Actor* actor = actorTable.find(actorId);
    if (!actor) {
        std::cout << "[Error] Actor ID " << actorId << " not found.\n";
        return;
    }
    Movie* movie = movieTable.find(movieId);
    if (!movie) {
        std::cout << "[Error] Movie ID " << movieId << " not found.\n";
        return;
    }

    movie->addActor(*actor);
    std::cout << "[Success] Actor \"" << actor->getName()
        << "\" added to movie \"" << movie->getTitle() << "\"\n";
}

//---------------------------------------------------------------------------
// Update actor details
//---------------------------------------------------------------------------
void MovieApp::updateActorDetails(int actorId, const std::string& newName, int newYearOfBirth) {
    if (!isAdmin) {
        std::cout << "[Error] Only administrators can update actor details.\n";
        return;
    }
    Actor* actor = actorTable.find(actorId);
    if (!actor) {
        std::cout << "[Error] Actor ID " << actorId << " not found.\n";
        return;
    }

    // FIX: Trim the incoming name
    std::string trimmedName = trim(newName);
    if (trimmedName.empty()) {
        std::cout << "[Error] Actor name cannot be empty.\n";
        return;
    }
    if (newYearOfBirth <= 0 || newYearOfBirth >= 2025) {
        std::cout << "[Error] Invalid year of birth.\n";
        return;
    }
    actor->setName(trimmedName.c_str());
    actor->setBirthYear(newYearOfBirth);

    actorTable.insert(*actor);
    std::cout << "[Success] Updated Actor ID " << actorId << "\n";
}


//---------------------------------------------------------------------------
// Update movie details
//---------------------------------------------------------------------------
void MovieApp::updateMovieDetails(int movieId,
    const std::string& newTitle,
    const std::string& newPlot,
    int newReleaseYear) {
    if (!isAdmin) {
        std::cout << "[Error] Only administrators can update movie details.\n";
        return;
    }

    Movie* movie = movieTable.find(movieId);
    if (!movie) {
        std::cout << "[Error] Movie ID " << movieId << " not found.\n";
        return;
    }

    std::string trimmedTitle = trimQuotes(newTitle); // Use trimQuotes

    if (trimmedTitle.empty()) {
        std::cout << "[Error] Movie title cannot be empty.\n";
        return;
    }
    if (newReleaseYear <= 1800 || newReleaseYear >= 2025) {
        std::cout << "[Error] Invalid release year.\n";
        return;
    }
    movie->setTitle(trimmedTitle.c_str());
    movie->setPlot(newPlot.c_str());
    movie->setReleaseYear(newReleaseYear);

    movieTable.insert(*movie);
    std::cout << "[Success] Updated Movie ID " << movieId << "\n";
}


//---------------------------------------------------------------------------
// findActorsByName
//---------------------------------------------------------------------------
void MovieApp::findActorsByName(const std::string& name, List<Actor>& result) const {
    // Example: iterate all actors in the hash table
    actorTable.forEach([&](const Actor& a) -> bool {
        if (std::strcmp(a.getName(), name.c_str()) == 0) {
            result.add(a);
        }
        return false; // keep going
        });
}

//---------------------------------------------------------------------------
// Display All Movies (alphabetical by title)
//---------------------------------------------------------------------------
static void mergeSortMovies(Movie* arr, int left, int right) {
    if (left >= right) return;
    int mid = (left + right) / 2;
    mergeSortMovies(arr, left, mid);
    mergeSortMovies(arr, mid + 1, right);

    // Merge
    int size = right - left + 1;
    // Value-initialize the temporary array to help with static analysis.
    Movie* temp = new Movie[size]();
    int i = left, j = mid + 1, k = 0;

    while (i <= mid && j <= right) {
        if (std::strcmp(arr[i].getTitle(), arr[j].getTitle()) <= 0) {
            assert(k < size);
            temp[k++] = arr[i++];
        }
        else {
            assert(k < size);
            temp[k++] = arr[j++];
        }
    }
    while (i <= mid) {
        assert(k < size);
        temp[k++] = arr[i++];
    }
    while (j <= right) {
        assert(k < size);
        temp[k++] = arr[j++];
    }
    for (int p = 0; p < size; ++p) {
        arr[left + p] = temp[p];
    }
    delete[] temp;
}


void MovieApp::displayAllMovies() const {
    if (movieTable.isEmpty()) {
        std::cout << "No movies found.\n";
        return;
    }

    // Collect all movies into an array
    static const int MAX_MOVIES = 2000;
    Movie* arr = new Movie[MAX_MOVIES];
    int count = 0;

    movieTable.forEach([&](const Movie& m) -> bool {
        if (count < MAX_MOVIES) {
            arr[count++] = m;
        }
        return false;
        });

    if (count == 0) {
        std::cout << "No movies found.\n";
        delete[] arr;
        return;
    }

    // Sort
    mergeSortMovies(arr, 0, count - 1);

    // Display
    std::cout << "All Movies (alphabetical):\n";
    for (int i = 0; i < count; ++i) {
        arr[i].displayDetails();
    }
    delete[] arr;
}

//---------------------------------------------------------------------------
// Display All Actors (alphabetical by name)
//---------------------------------------------------------------------------
static void mergeSortActors(Actor* arr, int left, int right) {
    if (left >= right) return;
    int mid = (left + right) / 2;
    mergeSortActors(arr, left, mid);
    mergeSortActors(arr, mid + 1, right);

    int size = right - left + 1;
    // Value-initialize the temporary array.
    Actor* temp = new Actor[size]();
    int i = left, j = mid + 1, k = 0;

    while (i <= mid && j <= right) {
        if (std::strcmp(arr[i].getName(), arr[j].getName()) <= 0) {
            assert(k < size);
            temp[k++] = arr[i++];
        }
        else {
            assert(k < size);
            temp[k++] = arr[j++];
        }
    }
    while (i <= mid) {
        assert(k < size);
        temp[k++] = arr[i++];
    }
    while (j <= right) {
        assert(k < size);
        temp[k++] = arr[j++];
    }
    for (int p = 0; p < size; ++p) {
        arr[left + p] = temp[p];
    }
    delete[] temp;
}

void MovieApp::displayAllActors() const {
    if (actorTable.isEmpty()) {
        std::cout << "No actors found.\n";
        return;
    }

    static const int MAX_ACTORS = 2000;
    Actor* arr = new Actor[MAX_ACTORS];
    int count = 0;

    // Collect
    actorTable.forEach([&](const Actor& a) -> bool {
        if (count < MAX_ACTORS) {
            arr[count++] = a;
        }
        return false;
        });
    if (count == 0) {
        std::cout << "No actors found.\n";
        delete[] arr;
        return;
    }

    // Sort
    mergeSortActors(arr, 0, count - 1);

    // Display
    std::cout << "All Actors (alphabetical):\n";
    for (int i = 0; i < count; ++i) {
        arr[i].displayDetails();
    }
    delete[] arr;
}

//---------------------------------------------------------------------------
// Display Actors by age range
//---------------------------------------------------------------------------
void MovieApp::displayActorsByAge(int minAge, int maxAge) const {
    bool foundAny = false;
    actorTable.forEach([&](const Actor& a) -> bool {
        int age = a.getAge();
        if (age >= minAge && age <= maxAge) {
            std::cout << a.getName() << " (Age=" << age << ")\n";
            foundAny = true;
        }
        return false;
        });
    if (!foundAny) {
        std::cout << "No actors found in age range [" << minAge << ", " << maxAge << "].\n";
    }
}

//---------------------------------------------------------------------------
// Display recent movies (released in last ~N years)
//---------------------------------------------------------------------------
void MovieApp::displayRecentMovies() const {
    // Example: last 3 years
    int currentYear = 2025; // or do actual time-based if you wish
    int cutoff = currentYear - 3;
    bool foundAny = false;

    movieTable.forEach([&](const Movie& m) -> bool {
        if (m.getReleaseYear() >= cutoff) {
            std::cout << m.getTitle() << " (" << m.getReleaseYear() << ")\n";
            foundAny = true;
        }
        return false;
        });
    if (!foundAny) {
        std::cout << "No movies found in the last 3 years.\n";
    }
}

//---------------------------------------------------------------------------
// Display movies of an actor
//---------------------------------------------------------------------------
void MovieApp::displayMoviesOfActor(int actorId) const {
    Actor* actPtr = actorTable.find(actorId);
    if (!actPtr) {
        std::cout << "Actor ID " << actorId << " not found.\n";
        return;
    }

    // We'll gather relevant movies into an array
    static const int MAX_MOVIES = 2000;
    Movie* arr = new Movie[MAX_MOVIES];
    int count = 0;

    movieTable.forEach([&](const Movie& m) -> bool {
        if (m.hasActor(actorId)) {
            if (count < MAX_MOVIES) {
                arr[count++] = m;
            }
        }
        return false;
        });
    if (count == 0) {
        std::cout << "No movies found for actor ID " << actorId << ".\n";
        delete[] arr;
        return;
    }

    // Sort the found movies by title (we can reuse mergeSortMovies)
    mergeSortMovies(arr, 0, count - 1);

    // Display
    std::cout << "Movies for actor ID " << actorId << ":\n";
    for (int i = 0; i < count; ++i) {
        std::cout << " - " << arr[i].getTitle()
            << " (" << arr[i].getReleaseYear() << ")\n";
    }
    delete[] arr;
}

//---------------------------------------------------------------------------
// Display actors in a given movie (by title)
//---------------------------------------------------------------------------
void MovieApp::displayActorsInMovie(const std::string& movieTitle) const {
    bool foundMovie = false;

    // We'll search all movies for the one with this title
    movieTable.forEach([&](const Movie& m) -> bool {
        if (std::strcmp(m.getTitle(), movieTitle.c_str()) == 0) {
            foundMovie = true;

            // Collect the cast from m.getActors() 
            // (which is still a List<Actor>)
            static const int MAX_ACTORS = 2000;
            Actor* actorArr = new Actor[MAX_ACTORS];
            int count = 0;

            m.getActors().display([&](const Actor& a) {
                if (count < MAX_ACTORS) {
                    actorArr[count++] = a;
                }
                return false;
                });

            // Sort by name
            mergeSortActors(actorArr, 0, count - 1);

            // Display
            std::cout << "Actors in \"" << movieTitle << "\":\n";
            for (int i = 0; i < count; ++i) {
                std::cout << " - " << actorArr[i].getName()
                    << " (Age: " << actorArr[i].getAge() << ")\n";
            }
            delete[] actorArr;
            return true; // Stop iteration after we find the matching movie
        }
        return false;
        });

    if (!foundMovie) {
        std::cout << "Movie \"" << movieTitle << "\" not found.\n";
    }
}


void MovieApp::displayActorsKnownBy(const std::string& actorName) const {
    // 1) Find the starting actor's ID by name
    int startActorId = -1;
    actorTable.forEach([&](const Actor& a) -> bool {
        if (std::strcmp(a.getName(), actorName.c_str()) == 0) {
            startActorId = a.getId();
            return true; // Stop iterating once found
        }
        return false;
        });

    // If not found, display error and stop
    if (startActorId == -1) {
        std::cout << "[Error] Actor \"" << actorName << "\" not found.\n";
        return;
    }

    // 2) Prepare structures to build the adjacency lists
    int* actorIds = new int[ActorGraph::MAX_ACTORS];
    List<int>* adjacencyLists = new List<int>[ActorGraph::MAX_ACTORS];
    int totalActors = 0;

    // 3) Build the actor graph using ActorGraph's method
    ActorGraph::buildActorGraph(actorTable, movieTable, actorIds, totalActors, adjacencyLists);

    // 4) Find the start index in actorIds
    int startIndex = ActorGraph::findActorIndexInArray(startActorId, actorIds, totalActors);
    if (startIndex == -1) {
        std::cout << "[Error] Could not map actor to index.\n";
        delete[] adjacencyLists;
        delete[] actorIds;
        return;
    }

    // 5) Use ActorGraph's BFS to find connected actors up to depth=2
    List<int> discovered = ActorGraph::findConnectedActors(startIndex, adjacencyLists, 2);

    if (discovered.isEmpty()) {
        std::cout << "No actors found that \"" << actorName << "\" knows (up to 2 levels).\n";
        delete[] adjacencyLists;
        delete[] actorIds;
        return;
    }

    // 6) Display the results
    std::cout << "Actors known by \"" << actorName << "\" (up to 2 levels):\n";
    discovered.display([&](int idx) {
        // idx is an index into actorIds[], so we retrieve the actual actorId
        if (idx < 0 || idx >= totalActors) return false;  // safety check
        int realActorId = actorIds[idx];

        // Look up the Actor* in the actorTable
        Actor* aPtr = actorTable.find(realActorId);
        if (aPtr) {
            std::cout << " - " << aPtr->getName() << "\n";
        }
        return false; // continue iteration over discovered
        });

    // Clean up
    delete[] adjacencyLists;
    delete[] actorIds;
}


//---------------------------------------------------------------------------
// Ratings
//---------------------------------------------------------------------------
void MovieApp::setActorRating(int actorId, int rating) {
    if (rating < 1 || rating > 10) {
        std::cout << "[Error] Invalid rating. Must be between 1 and 10.\n";
        return;
    }
    Actor* a = actorTable.find(actorId);
    if (!a) {
        std::cout << "[Error] Actor ID " << actorId << " not found.\n";
        return;
    }
    a->setRating(rating);
    actorRatings.insert(*a);
    // Re-insert to update in the HashTable
    actorTable.insert(*a);
    std::cout << "[Success] Actor (ID=" << actorId << ") rating updated to " << rating << "\n";
}

void MovieApp::setMovieRating(int movieId, int rating) {
    if (rating < 1 || rating > 10) {
        std::cout << "[Error] Invalid rating. Must be between 1 and 10.\n";
        return;
    }
    Movie* m = movieTable.find(movieId);
    if (!m) {
        std::cout << "[Error] Movie ID " << movieId << " not found.\n";
        return;
    }
    m->setRating(rating);
    movieRatings.insert(*m);
    movieTable.insert(*m);
    std::cout << "[Success] Movie (ID=" << movieId << ") rating updated to " << rating << "\n";
}

void MovieApp::recommendMoviesByRating(int minRating, int maxRating) const {
    std::cout << "Movies with rating between " << minRating << " and " << maxRating << ":\n";
    movieRatings.displayMoviesInRange(minRating, maxRating);
}

void MovieApp::recommendActorsByRating(int minRating, int maxRating) const {
    std::cout << "Actors with rating between " << minRating << " and " << maxRating << ":\n";
    actorRatings.displayActorsInRange(minRating, maxRating);
}

// Helper function to print test headers
static void printTestHeader(const std::string& testName) {
    std::cout << "\n--- " << testName << " ---\n";
}

// Helper function to print test results
static void printTestResult(const std::string& testName, bool passed, const std::string& details = "") {
    if (passed) {
        std::cout << "[PASS] " << testName << "\n";
    }
    else {
        std::cout << "[FAIL] " << testName << " - " << details << "\n";
    }
}


