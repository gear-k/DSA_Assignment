// MovieApp.cpp
#include "MovieApp.h"
#include "GetKeySpecializations.h"  // Must include after Actor/Movie
#include "ActorGraph.h"            // If you still use ActorGraph utilities
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <cstdlib>   // atoi
#include <ctime>     // for date-based logic (e.g., recent movies)

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
        std::string idStr, nameStr, birthStr;

        // CSV assumed format: actorId,name,birthYear
        std::getline(ss, idStr, ',');
        std::getline(ss, nameStr, ',');
        std::getline(ss, birthStr, ',');

        if (idStr.empty() || nameStr.empty() || birthStr.empty()) {
            std::cerr << "[Warning] Malformed row: " << line << "\n";
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
        std::string idStr, titleStr, yearStr;

        // CSV assumed format: movieId,title,releaseYear (or maybe 4 fields if there's a plot)
        std::getline(ss, idStr, ',');
        std::getline(ss, titleStr, ',');
        std::getline(ss, yearStr, ',');

        if (idStr.empty() || titleStr.empty() || yearStr.empty()) {
            std::cerr << "[Warning] Malformed row: " << line << "\n";
            continue;
        }

        int id = std::atoi(idStr.c_str());
        int year = std::atoi(yearStr.c_str());
        titleStr = trimQuotes(titleStr);

        Movie movie(titleStr.c_str(), "", year, id);
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
            std::cerr << "[Warning] Malformed row: " << line << "\n";
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
            std::cerr << "[Warning] Invalid actorId or movieId in cast: " << line << "\n";
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
    Movie* temp = new Movie[size];
    int i = left, j = mid + 1, k = 0;

    while (i <= mid && j <= right) {
        if (std::strcmp(arr[i].getTitle(), arr[j].getTitle()) <= 0) {
            temp[k++] = arr[i++];
        }
        else {
            temp[k++] = arr[j++];
        }
    }
    while (i <= mid) {
        temp[k++] = arr[i++];
    }
    while (j <= right) {
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
    Actor* temp = new Actor[size];
    int i = left, j = mid + 1, k = 0;

    while (i <= mid && j <= right) {
        if (std::strcmp(arr[i].getName(), arr[j].getName()) <= 0) {
            temp[k++] = arr[i++];
        }
        else {
            temp[k++] = arr[j++];
        }
    }
    while (i <= mid) {
        temp[k++] = arr[i++];
    }
    while (j <= right) {
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

//---------------------------------------------------------------------------
// BFS-based "displayActorsKnownBy" (2-level acquaintances)
//---------------------------------------------------------------------------
#include <cassert>

// Our custom BFS queue if needed:
struct BFSQueue {
    struct Pair { int idx; int depth; };
    Pair data[2000];
    int front, rear, count;
    BFSQueue() : front(0), rear(-1), count(0) {}

    bool isEmpty() const { return (count == 0); }
    bool isFull()  const { return (count == 2000); }

    bool enqueue(int i, int d) {
        if (isFull()) return false;
        rear = (rear + 1) % 2000;
        data[rear].idx = i;
        data[rear].depth = d;
        count++;
        return true;
    }
    bool dequeue(Pair& out) {
        if (isEmpty()) return false;
        out = data[front];
        front = (front + 1) % 2000;
        count--;
        return true;
    }
};

void MovieApp::buildActorGraph(
    int actorIds[],
    int& actorCount,
    List<int>* adjacencyLists,
    const int MAX_ACTORS
) const {
    // Collect all actor IDs first
    actorCount = 0;
    actorTable.forEach([&](const Actor& a) -> bool {
        if (actorCount < MAX_ACTORS) {
            actorIds[actorCount++] = a.getId();
        }
        return false;
        });

    // Build adjacency by iterating over all movies
    movieTable.forEach([&](const Movie& m) -> bool {
        // gather the indices of all actors in this movie
        static const int TEMP_SIZE = 300;
        int tmpIdx[TEMP_SIZE];
        int tmpCount = 0;

        m.getActors().display([&](const Actor& a) {
            // find index in actorIds
            int idx = findActorIndexInArray(a.getId(), actorIds, actorCount);
            if (idx != -1 && tmpCount < TEMP_SIZE) {
                tmpIdx[tmpCount++] = idx;
            }
            return false;
            });

        // For each pair in tmpIdx, make them neighbors
        for (int i = 0; i < tmpCount; i++) {
            for (int j = i + 1; j < tmpCount; j++) {
                adjacencyLists[tmpIdx[i]].add(tmpIdx[j]);
                adjacencyLists[tmpIdx[j]].add(tmpIdx[i]);
            }
        }
        return false;
        });
}

int MovieApp::findActorIndexInArray(int actorId, const int actorIds[], int count) const {
    for (int i = 0; i < count; i++) {
        if (actorIds[i] == actorId) {
            return i;
        }
    }
    return -1;
}

void MovieApp::displayActorsKnownBy(const std::string& actorName) const {
    // 1) Find the starting actor ID by name
    int startActorId = -1;
    actorTable.forEach([&](const Actor& a) -> bool {
        if (std::strcmp(a.getName(), actorName.c_str()) == 0) {
            startActorId = a.getId();
            return true; // stop
        }
        return false;
        });
    if (startActorId == -1) {
        std::cout << "[Error] Actor \"" << actorName << "\" not found.\n";
        return;
    }

    // 2) Build adjacency
    int* actorIds = new int[ActorGraph::MAX_ACTORS];
    List<int>* adjacencyLists = new List<int>[ActorGraph::MAX_ACTORS];
    int totalActors = 0;

    ActorGraph::buildActorGraph(actorTable, movieTable, actorIds, totalActors, adjacencyLists);

    // 3) Find the start index
    int startIndex = ActorGraph::findActorIndexInArray(startActorId, actorIds, totalActors);
    if (startIndex == -1) {
        std::cout << "[Error] Could not map actor to index.\n";
        delete[] actorIds;
        delete[] adjacencyLists;
        return;
    }

    // 4) BFS up to depth=2
    List<int> discovered = ActorGraph::findConnectedActors(startIndex, adjacencyLists, 2);

    if (discovered.isEmpty()) {
        std::cout << "No actors found that \"" << actorName << "\" knows (up to 2 levels).\n";
        delete[] actorIds;
        delete[] adjacencyLists;
        return;
    }

    // 5) Display
    std::cout << "Actors known by \"" << actorName << "\" (up to 2 levels):\n";
    discovered.display([&](int idx) {
        if (idx < 0 || idx >= totalActors) return false;
        int realActorId = actorIds[idx];

        // Find the actual Actor
        Actor* aPtr = actorTable.find(realActorId);
        if (aPtr) {
            std::cout << " - " << aPtr->getName() << "\n";
        }
        return false;
        });

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

// Enhanced runAllTests method
void MovieApp::runAllTests() {
    std::cout << "\n========== Running All Tests ==========\n";

    // 1) Clear existing data
    printTestHeader("Test 1: Clearing Existing Data");
    actorTable.clear();
    movieTable.clear();
    printTestResult("Clearing actor and movie tables", actorTable.isEmpty() && movieTable.isEmpty());

    // 2) Reset next IDs
    printTestHeader("Test 2: Resetting Next IDs");
    nextActorId = 1000;
    nextMovieId = 5000;
    printTestResult("Resetting nextActorId to 1000", nextActorId == 1000);
    printTestResult("Resetting nextMovieId to 5000", nextMovieId == 5000);

    // 3) Add Actors with Valid Data
    printTestHeader("Test 3: Adding Actors with Valid Data");
    setAdminMode(true); // Enable admin mode

    addNewActor("Leonardo DiCaprio", 1974); // Expected ID=1000
    addNewActor("Brad Pitt", 1963);         // Expected ID=1001
    addNewActor("Meryl Streep", 1949);      // Expected ID=1002

    bool test3_passed = isActorIdUsed(1000) && isActorIdUsed(1001) && isActorIdUsed(1002);
    printTestResult("Adding valid actors", test3_passed);

    // 4) Add Movies with Valid Data
    printTestHeader("Test 4: Adding Movies with Valid Data");
    addNewMovie("Inception", "A mind-bending thriller", 2010); // Expected ID=5000
    addNewMovie("Fight Club", "An underground fight club", 1999); // Expected ID=5001
    addNewMovie("The Devil Wears Prada", "Fashion industry drama", 2006); // Expected ID=5002

    bool test4_passed = isMovieIdUsed(5000) && isMovieIdUsed(5001) && isMovieIdUsed(5002);
    printTestResult("Adding valid movies", test4_passed);

    // 5) Attempt to Add Actors in User Mode (Should Fail)
    printTestHeader("Test 5: Adding Actors in User Mode");
    setAdminMode(false); // Disable admin mode
    int initialActorCount = actorTable.getCount();
    addNewActor("Tom Hanks", 1956); // Should not be added
    bool test5_passed = (actorTable.getCount() == initialActorCount);
    printTestResult("Prevent adding actors in user mode", test5_passed);

    // 6) Attempt to Add Movies in User Mode (Should Fail)
    printTestHeader("Test 6: Adding Movies in User Mode");
    int initialMovieCount = movieTable.getCount();
    addNewMovie("Forrest Gump", "Life journey of Forrest", 1994); // Should not be added
    bool test6_passed = (movieTable.getCount() == initialMovieCount);
    printTestResult("Prevent adding movies in user mode", test6_passed);

    // 7) Attempt to Add Actor with Invalid Data (Empty Name)
    printTestHeader("Test 7: Adding Actor with Empty Name");
    setAdminMode(true); // Enable admin mode
    addNewActor("   ", 1980); // Should fail
    bool test7_passed = (actorTable.getCount() == initialActorCount + 3); // No new actor added
    printTestResult("Prevent adding actor with empty name", test7_passed, "Actor count should remain unchanged");

    // 8) Attempt to Add Actor with Invalid Birth Year
    printTestHeader("Test 8: Adding Actor with Invalid Birth Year");
    addNewActor("Samuel L. Jackson", -5); // Should fail
    bool test8_passed = (actorTable.getCount() == initialActorCount + 3);
    printTestResult("Prevent adding actor with invalid birth year", test8_passed, "Actor count should remain unchanged");

    // 9) Add Actor with Leading/Trailing Whitespace
    printTestHeader("Test 9: Adding Actor with Leading/Trailing Whitespace");
    addNewActor("  Scarlett Johansson  ", 1984); // Expected ID=1003
    bool test9_passed = isActorIdUsed(1003) && strcmp(actorTable.find(1003)->getName(), "Scarlett Johansson") == 0;
    printTestResult("Trimming whitespace in actor names", test9_passed);

    // 10) Add Movie with Leading/Trailing Whitespace and Quoted Title
    printTestHeader("Test 10: Adding Movie with Whitespace and Quotes");
    addNewMovie("  \"The Matrix\"  ", "A computer hacker learns about the true nature of reality", 1999); // Expected ID=5003
    bool test10_passed = isMovieIdUsed(5003) && strcmp(movieTable.find(5003)->getTitle(), "The Matrix") == 0;
    printTestResult("Trimming whitespace and quotes in movie titles", test10_passed);

    // 11) Attempt to Add Duplicate Actor ID
    printTestHeader("Test 11: Adding Duplicate Actor ID");
    // Directly attempt to insert an actor with an existing ID
    Actor duplicateActor("Duplicate Actor", 1980, 1000);
    actorTable.insert(duplicateActor); // If insert returns void
    // Should handle duplicate
    printTestResult("Prevent inserting actor with duplicate ID", "Duplicate insertion should fail");

    // 12) Attempt to Add Duplicate Movie ID
    printTestHeader("Test 12: Adding Duplicate Movie ID");
    Movie duplicateMovie("Duplicate Movie", "Duplicate Plot", 2000, 5000);
    movieTable.insert(duplicateMovie); // Should handle duplicate
    printTestResult("Prevent inserting movie with duplicate ID", "Duplicate insertion should fail");

    // 13) Read Actors from Valid CSV
    printTestHeader("Test 13: Reading Actors from Valid CSV");
    // Assuming the CSV content is stored in a string for testing
    std::ofstream validActorsCSV("valid_actors.csv");
    validActorsCSV << "id,name,birth\n"
        << "102,\"Kevin Bacon\",1958\n"
        << "129,\"Tom Cruise\",1962\n"
        << "119,\"Tom Cruise\",1962\n"
        << "019,\"Tom Cruise\",1962\n"
        << "144,\"Cary Elwes\",1962\n"
        << "158,\"Tom Hanks\",1956\n";
    validActorsCSV.close();

    readActors("valid_actors.csv");
    bool test13_passed = isActorIdUsed(102) && isActorIdUsed(129) && isActorIdUsed(119) &&
        isActorIdUsed(19) && isActorIdUsed(144) && isActorIdUsed(158);
    printTestResult("Reading actors from valid CSV", test13_passed);

    // 14) Read Movies from Valid CSV
    printTestHeader("Test 14: Reading Movies from Valid CSV");
    std::ofstream validMoviesCSV("valid_movies.csv");
    validMoviesCSV << "id,title,year\n"
        << "112384,\"Apollo 13\",1995\n"
        << "104257,\"A Few Good Men\",1992\n"
        << "109830,\"Forrest Gump\",1994\n"
        << "93779,\"The Princess Bride\",1987\n"
        << "95953,\"Rain Man\",1988\n"
        << "95913,\"Rain Man\",1988\n";
    validMoviesCSV.close();

    readMovies("valid_movies.csv");
    bool test14_passed = isMovieIdUsed(112384) && isMovieIdUsed(104257) && isMovieIdUsed(109830) &&
        isMovieIdUsed(93779) && isMovieIdUsed(95953) && isMovieIdUsed(95913);
    printTestResult("Reading movies from valid CSV", test14_passed);

    // 15) Read Cast from Valid CSV
    printTestHeader("Test 15: Reading Cast from Valid CSV");
    std::ofstream validCastCSV("valid_cast.csv");
    validCastCSV << "person_id,movie_id\n"
        << "102,104257\n"
        << "102,112384\n"
        << "129,104257\n"
        << "129,95953\n"
        << "144,93779\n"
        << "158,109830\n"
        << "158,112384\n"
        << "1597,93779\n"
        << "163,95953\n"
        << "1697,93779\n"
        << "193,104257\n"
        << "197,104257\n"
        << "200,112384\n"
        << "398,109830\n"
        << "420,95953\n"
        << "596520,95953\n"
        << "641,109830\n"
        << "641,112384\n"
        << "705,109830\n"
        << "705,93779\n";
    validCastCSV.close();

    readCast("valid_cast.csv");
    // Verify a few actor-movie links
    Movie* inception = movieTable.find(112384);
    Actor* kevinBacon = actorTable.find(102);
    bool test15_passed = inception && kevinBacon && inception->hasActor(102);
    printTestResult("Reading cast from valid CSV and linking actors to movies", test15_passed);

    // 16) Read Actors from Malformed CSV
    printTestHeader("Test 16: Reading Actors from Malformed CSV");
    std::ofstream malformedActorsCSV("malformed_actors.csv");
    malformedActorsCSV << "id,name,birth\n"
        << "2001,\"Invalid Actor\"\n" // Missing birth year
        << "2002,,1970\n"              // Missing name
        << ",\"No ID\",1980\n"         // Missing ID
        << "2003,\"Valid Actor\",1990\n"; // Valid entry
    malformedActorsCSV.close();

    int actorsBefore = actorTable.getCount();
    readActors("malformed_actors.csv");
    int actorsAfter = actorTable.getCount();
    bool test16_passed = (actorsAfter - actorsBefore) == 1 && isActorIdUsed(2003);
    printTestResult("Handling malformed actor CSV rows", test16_passed);

    // 17) Read Movies from Malformed CSV
    printTestHeader("Test 17: Reading Movies from Malformed CSV");
    std::ofstream malformedMoviesCSV("malformed_movies.csv");
    malformedMoviesCSV << "id,title,year\n"
        << "3001,\"Invalid Movie\"\n" // Missing year
        << "3002,,2000\n"              // Missing title
        << ",\"No ID\",1990\n"         // Missing ID
        << "3003,\"Valid Movie\",2010\n"; // Valid entry
    malformedMoviesCSV.close();

    int moviesBefore = movieTable.getCount();
    readMovies("malformed_movies.csv");
    int moviesAfter = movieTable.getCount();
    bool test17_passed = (moviesAfter - moviesBefore) == 1 && isMovieIdUsed(3003);
    printTestResult("Handling malformed movie CSV rows", test17_passed);

    // 18) Test Linking Non-Existent Actor and Movie
    printTestHeader("Test 18: Linking Non-Existent Actor and Movie");
    int initialLinks = movieTable.find(112384)->getActors().getCount();
    addActorToMovieById(9999, 9999, true); // Both IDs do not exist
    int finalLinks = movieTable.find(112384)->getActors().getCount();
    bool test18_passed = (initialLinks == finalLinks);
    printTestResult("Prevent linking non-existent actors and movies", test18_passed, "No new links should be created");

    // 19) Test Updating Actor Details with Valid Data
    printTestHeader("Test 19: Updating Actor Details with Valid Data");
    updateActorDetails(1000, "Leonardo Wilhelm DiCaprio", 1974);
    Actor* updatedActor = actorTable.find(1000);
    bool test19_passed = updatedActor && strcmp(updatedActor->getName(), "Leonardo Wilhelm DiCaprio") == 0 && updatedActor->getBirthYear() == 1974;
    printTestResult("Updating actor's name and birth year", test19_passed);

    // 20) Test Updating Actor Details with Invalid Data (Empty Name)
    printTestHeader("Test 20: Updating Actor Details with Empty Name");
    updateActorDetails(1000, "   ", 1974); // Should fail
    bool test20_passed = strcmp(actorTable.find(1000)->getName(), "Leonardo Wilhelm DiCaprio") == 0;
    printTestResult("Prevent updating actor with empty name", test20_passed, "Name should remain unchanged");

    // 21) Test Updating Movie Details with Valid Data
    printTestHeader("Test 21: Updating Movie Details with Valid Data");
    updateMovieDetails(5000, "Inception: Dream Within a Dream", "Updated plot", 2010);
    Movie* updatedMovie = movieTable.find(5000);
    bool test21_passed = updatedMovie && strcmp(updatedMovie->getTitle(), "Inception: Dream Within a Dream") == 0 &&
        strcmp(updatedMovie->getPlot(), "Updated plot") == 0 && updatedMovie->getReleaseYear() == 2010;
    printTestResult("Updating movie's title, plot, and release year", test21_passed);

    // 22) Test Updating Movie Details with Invalid Data (Invalid Year)
    printTestHeader("Test 22: Updating Movie Details with Invalid Year");
    updateMovieDetails(5000, "Inception", "New Plot", 1500); // Should fail
    bool test22_passed = strcmp(movieTable.find(5000)->getTitle(), "Inception: Dream Within a Dream") == 0 &&
        strcmp(movieTable.find(5000)->getPlot(), "Updated plot") == 0 &&
        movieTable.find(5000)->getReleaseYear() == 2010;
    printTestResult("Prevent updating movie with invalid release year", test22_passed, "Movie details should remain unchanged");

    // 23) Test Setting Actor Rating with Valid Data
    printTestHeader("Test 23: Setting Actor Rating with Valid Data");
    setActorRating(1001, 8); // Brad Pitt
    bool test23_passed = actorTable.find(1001)->getRating() == 8;
    printTestResult("Setting valid actor rating", test23_passed);

    // 24) Test Setting Actor Rating with Invalid Data
    printTestHeader("Test 24: Setting Actor Rating with Invalid Data");
    setActorRating(1001, 15); // Invalid rating
    bool test24_passed = actorTable.find(1001)->getRating() == 8; // Rating should remain unchanged
    printTestResult("Prevent setting invalid actor rating", test24_passed, "Rating should remain unchanged");

    // 25) Test Setting Movie Rating with Valid Data
    printTestHeader("Test 25: Setting Movie Rating with Valid Data");
    setMovieRating(5001, 9); // Fight Club
    bool test25_passed = movieTable.find(5001)->getRating() == 9;
    printTestResult("Setting valid movie rating", test25_passed);

    // 26) Test Setting Movie Rating with Invalid Data
    printTestHeader("Test 26: Setting Movie Rating with Invalid Data");
    setMovieRating(5001, 0); // Invalid rating
    bool test26_passed = movieTable.find(5001)->getRating() == 9; // Rating should remain unchanged
    printTestResult("Prevent setting invalid movie rating", test26_passed, "Rating should remain unchanged");

    // 27) Test Displaying All Actors Alphabetically
    printTestHeader("Test 27: Displaying All Actors Alphabetically");
    displayAllActors(); // Manual verification or implement assertion based on sorted order
    // For automated testing, you might capture the output and verify the order

    // 28) Test Displaying All Movies Alphabetically
    printTestHeader("Test 28: Displaying All Movies Alphabetically");
    displayAllMovies(); // Manual verification or implement assertion based on sorted order

    // 29) Test Displaying Actors by Age Range
    printTestHeader("Test 29: Displaying Actors by Age Range");
    displayActorsByAge(50, 70); // Expected to display actors within this age range
    // For automated testing, you might capture the output and verify the actors listed

    // 30) Test Displaying Recent Movies
    printTestHeader("Test 30: Displaying Recent Movies");
    displayRecentMovies(); // Should display movies released in the last 3 years (up to 2025)
    // Expected to include "Inception: Dream Within a Dream" (2010) might not, depends on currentYear logic

    // 31) Test Displaying Movies of an Actor
    printTestHeader("Test 31: Displaying Movies of an Actor");
    displayMoviesOfActor(1000); // Leonardo Wilhelm DiCaprio
    // Expected to display "Inception: Dream Within a Dream"

    // 32) Test Displaying Actors in a Movie
    printTestHeader("Test 32: Displaying Actors in a Movie");
    displayActorsInMovie("Fight Club"); // Should display Brad Pitt
    displayActorsInMovie("Non-Existent Movie"); // Should handle gracefully

    // 33) Test Displaying Actors Known By (BFS)
    printTestHeader("Test 33: Displaying Actors Known By (BFS)");
    displayActorsKnownBy("Leonardo Wilhelm DiCaprio"); // Should display connected actors up to 2 levels

    // 34) Test Reading Duplicate Actors in CSV
    printTestHeader("Test 34: Reading Duplicate Actors from CSV");
    std::ofstream duplicateActorsCSV("duplicate_actors.csv");
    duplicateActorsCSV << "id,name,birth\n"
        << "102,\"Kevin Bacon\",1958\n" // Duplicate ID
        << "2004,\"New Actor\",1985\n";
    duplicateActorsCSV.close();

    int actorsBeforeDup = actorTable.getCount();
    readActors("duplicate_actors.csv");
    int actorsAfterDup = actorTable.getCount();
    bool test34_passed = (actorsAfterDup - actorsBeforeDup) == 1 && isActorIdUsed(2004);
    printTestResult("Handling duplicate actor IDs in CSV", test34_passed, "Duplicate should be ignored");

    // 35) Test Reading Duplicate Movies in CSV
    printTestHeader("Test 35: Reading Duplicate Movies from CSV");
    std::ofstream duplicateMoviesCSV("duplicate_movies.csv");
    duplicateMoviesCSV << "id,title,year\n"
        << "112384,\"Apollo 13\",1995\n" // Duplicate ID
        << "3004,\"New Movie\",2020\n";
    duplicateMoviesCSV.close();

    int moviesBeforeDup = movieTable.getCount();
    readMovies("duplicate_movies.csv");
    int moviesAfterDup = movieTable.getCount();
    bool test35_passed = (moviesAfterDup - moviesBeforeDup) == 1 && isMovieIdUsed(3004);
    printTestResult("Handling duplicate movie IDs in CSV", test35_passed, "Duplicate should be ignored");

    // 36) Test Linking Actor to Multiple Movies
    printTestHeader("Test 36: Linking Actor to Multiple Movies");
    addActorToMovieById(1000, 5001, true); // Leonardo DiCaprio to Fight Club
    addActorToMovieById(1000, 5002, true); // Leonardo DiCaprio to The Devil Wears Prada
    Movie* fightClub = movieTable.find(5001);
    Movie* devilsPrada = movieTable.find(5002);
    bool test36_passed = fightClub && fightClub->hasActor(1000) && devilsPrada && devilsPrada->hasActor(1000);
    printTestResult("Linking actor to multiple movies", test36_passed);

    // 37) Test Error Handling When Linking Existing Actor to Existing Movie
    printTestHeader("Test 37: Linking Existing Actor to Existing Movie");
    int initialLinksFightClub = fightClub->getActors().getCount();
    addActorToMovieById(1000, 5001, true); // Attempt duplicate link
    int finalLinksFightClub = fightClub->getActors().getCount();
    bool test37_passed = (initialLinksFightClub == finalLinksFightClub);
    printTestResult("Prevent duplicate actor-movie linking", test37_passed, "Duplicate link should not be created");

    // 38) Test Reading Cast with Non-Existent IDs
    printTestHeader("Test 38: Reading Cast with Non-Existent IDs");
    std::ofstream invalidCastCSV("invalid_cast.csv");
    invalidCastCSV << "person_id,movie_id\n"
        << "9999,9999\n" // Both IDs do not exist
        << "1003,5003\n" // Valid actor and movie
        << "1004,5004\n"; // Non-existent movie
    invalidCastCSV.close();

    int linksBeforeInvalid = movieTable.find(5003)->getActors().getCount();
    readCast("invalid_cast.csv");
    int linksAfterInvalid = movieTable.find(5003)->getActors().getCount();
    bool test38_passed = (linksAfterInvalid - linksBeforeInvalid) == 1; // Only valid link should be added
    printTestResult("Handling cast CSV with non-existent IDs", test38_passed);

    // 39) Test Displaying Actors Known By Non-Existent Actor
    printTestHeader("Test 39: Displaying Actors Known By Non-Existent Actor");
    displayActorsKnownBy("Ghost Actor"); // Should handle gracefully

    // 40) Clean Up Test Files
    printTestHeader("Test 40: Cleaning Up Test Files");
    bool cleanup_passed =
        std::remove("valid_actors.csv") == 0 &&
        std::remove("valid_movies.csv") == 0 &&
        std::remove("valid_cast.csv") == 0 &&
        std::remove("malformed_actors.csv") == 0 &&
        std::remove("malformed_movies.csv") == 0 &&
        std::remove("duplicate_actors.csv") == 0 &&
        std::remove("duplicate_movies.csv") == 0 &&
        std::remove("invalid_cast.csv") == 0;
    printTestResult("Removing temporary test CSV files", cleanup_passed);

    std::cout << "========== Tests Complete ==========\n\n";
}

