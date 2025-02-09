/***************************************************************************
 * MovieApp.cpp
 *
 * Team:            Gearoid, Cedric
 * Group:           1
 * Student IDs:     S10241866, S10241549
 *
 * Features Highlight:
 *   - CSV reading and parsing for actors, movies, and casts.
 *   - Case-insensitive string comparisons and trimming utilities.
 *   - Dynamic arrays and merge sort for movies and actors.
 *   - Administrative and user operations including ratings and recommendations.
 ***************************************************************************/

#include "MovieApp.h"
#include "ActorGraph.h"            // If you still use ActorGraph utilities
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <cstdlib>     // For atoi
#include <ctime>       // For date-based logic (e.g., recent movies)
#include <cassert>     // For debugging purposes
#include <limits>      // For std::numeric_limits

 // ---------------------------------------------------------------------------
 // Helper Functions
 // ---------------------------------------------------------------------------

 /**
  * @brief Case-insensitive comparison of two C-strings.
  *
  * Compares characters in a case-insensitive manner until a difference is found
  * or both strings terminate.
  *
  * @param s1 First C-string.
  * @param s2 Second C-string.
  * @return Negative if s1 < s2, positive if s1 > s2, zero if equal.
  */
static int caseInsensitiveCompareStrings(const char* s1, const char* s2) {
    while (*s1 && *s2) {
        unsigned char c1 = static_cast<unsigned char>(std::tolower(*s1));
        unsigned char c2 = static_cast<unsigned char>(std::tolower(*s2));
        if (c1 < c2) return -1;
        if (c1 > c2) return 1;
        ++s1;
        ++s2;
    }
    unsigned char c1 = static_cast<unsigned char>(std::tolower(*s1));
    unsigned char c2 = static_cast<unsigned char>(std::tolower(*s2));
    if (c1 < c2) return -1;
    if (c1 > c2) return 1;
    return 0;
}

/**
 * @brief Trims leading/trailing quotes and spaces from a string.
 *
 * @param str The input string.
 * @return The trimmed string.
 */
static std::string trimQuotes(const std::string& str) {
    size_t start = str.find_first_not_of(" \"");
    size_t end = str.find_last_not_of(" \"");
    if (start == std::string::npos || end == std::string::npos) {
        return "";
    }
    return str.substr(start, end - start + 1);
}

/**
 * @brief Trims leading and trailing spaces and tabs from a string.
 *
 * @param str The input string.
 * @return The trimmed string.
 */
static std::string trim(const std::string& str) {
    size_t start = str.find_first_not_of(" \t");
    size_t end = str.find_last_not_of(" \t");
    if (start == std::string::npos || end == std::string::npos) {
        return "";
    }
    return str.substr(start, end - start + 1);
}

// ---------------------------------------------------------------------------
// MovieApp Constructor and Admin Mode Methods
// ---------------------------------------------------------------------------

/**
 * @brief Constructs a new MovieApp instance.
 *
 * Initializes the actor and movie hash tables with capacity 2000, sets the
 * next available IDs, and defaults to user mode.
 */
MovieApp::MovieApp()
    : actorTable(2000),
    movieTable(2000),
    nextActorId(1000),
    nextMovieId(5000),
    isAdmin(false)
{
}

/**
 * @brief Sets the application mode to admin or user.
 *
 * @param admin True to enable admin mode; false for user mode.
 */
void MovieApp::setAdminMode(bool admin) {
    isAdmin = admin;
    if (isAdmin) {
        std::cout << "[Mode] Administrator mode enabled.\n";
    }
    else {
        std::cout << "[Mode] User mode enabled.\n";
    }
}

/**
 * @brief Checks if the application is in admin mode.
 *
 * @return true if admin mode is enabled; false otherwise.
 */
bool MovieApp::isAdminMode() const {
    return isAdmin;
}

// ---------------------------------------------------------------------------
// CSV Reading Methods
// ---------------------------------------------------------------------------

/**
 * @brief Reads actor data from a CSV file.
 *
 * Parses each row to extract actor ID, name, and birth year, then creates
 * Actor objects and inserts them into the actor hash table.
 *
 * @param filename The path to the CSV file.
 */
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
        std::getline(ss, idStr, ',');
        std::getline(ss, nameStr, ',');
        std::getline(ss, birthStr, ',');

        // Skip malformed rows.
        if (idStr.empty() || nameStr.empty() || birthStr.empty()) {
            continue;
        }

        int id = std::atoi(idStr.c_str());
        int birth = std::atoi(birthStr.c_str());
        nameStr = trimQuotes(nameStr);

        Actor actor(nameStr.c_str(), birth, id);
        actorTable.insert(actor);
        if (id >= nextActorId) {
            nextActorId = id + 1;
        }
    }
    fin.close();
}

/**
 * @brief Reads movie data from a CSV file.
 *
 * Parses each row to extract movie ID, title, plot, and release year,
 * creates Movie objects, and inserts them into the movie hash table.
 *
 * @param filename The path to the CSV file.
 */
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
        std::string idStr, titleStr, plotStr, yearStr;
        std::getline(ss, idStr, ',');
        std::getline(ss, titleStr, ',');
        std::getline(ss, plotStr, ',');
        std::getline(ss, yearStr, ',');

        // Validate basic row structure.
        if (idStr.empty() || titleStr.empty() || plotStr.empty() || yearStr.empty()) {
            std::cerr << "[Warning] Malformed row: " << line << "\n";
            continue;
        }

        int id = std::atoi(idStr.c_str());
        int year = std::atoi(yearStr.c_str());
        titleStr = trimQuotes(titleStr);
        plotStr = trimQuotes(plotStr);

        Movie movie(titleStr.c_str(), plotStr.c_str(), year, id);
        movieTable.insert(movie);
        if (id >= nextMovieId) {
            nextMovieId = id + 1;
        }
    }
    fin.close();
}

/**
 * @brief Reads cast data from a CSV file.
 *
 * Parses each row to extract actor and movie IDs, then links actors to movies
 * by adding the actor to the corresponding movie's cast.
 *
 * @param filename The path to the CSV file.
 */
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
        std::getline(ss, actorIdStr, ',');
        std::getline(ss, movieIdStr, ',');

        if (actorIdStr.empty() || movieIdStr.empty()) {
            continue;
        }

        int aId = std::atoi(actorIdStr.c_str());
        int mId = std::atoi(movieIdStr.c_str());

        // Link the actor and movie if both exist.
        Actor* actor = actorTable.find(aId);
        Movie* movie = movieTable.find(mId);
        if (actor && movie) {
            movie->addActor(*actor);
        }
    }
    fin.close();
}

// ---------------------------------------------------------------------------
// Next ID Accessors and ID Checks
// ---------------------------------------------------------------------------

/**
 * @brief Gets the next available actor ID.
 *
 * @return The next actor ID.
 */
int MovieApp::getNextActorId() const {
    return nextActorId;
}

/**
 * @brief Gets the next available movie ID.
 *
 * @return The next movie ID.
 */
int MovieApp::getNextMovieId() const {
    return nextMovieId;
}

/**
 * @brief Checks whether an actor ID is already used.
 *
 * @param id The actor ID to check.
 * @return true if the actor ID is used; false otherwise.
 */
bool MovieApp::isActorIdUsed(int id) const {
    return (actorTable.find(id) != nullptr);
}

/**
 * @brief Checks whether a movie ID is already used.
 *
 * @param id The movie ID to check.
 * @return true if the movie ID is used; false otherwise.
 */
bool MovieApp::isMovieIdUsed(int id) const {
    return (movieTable.find(id) != nullptr);
}

// ---------------------------------------------------------------------------
// Adding and Updating Actors and Movies
// ---------------------------------------------------------------------------

/**
 * @brief Adds a new actor to the system.
 *
 * Validates input, ensures a unique actor ID, creates an Actor, and inserts it
 * into the actor hash table.
 *
 * @param name The name of the actor.
 * @param birthYear The birth year of the actor.
 */
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
    if (birthYear <= 0 || birthYear > 2025) {
        std::cout << "[Error] Invalid birth year.\n";
        return;
    }
    // Ensure a unique actor ID.
    while (isActorIdUsed(nextActorId)) {
        ++nextActorId;
    }
    Actor actor(trimmedName.c_str(), birthYear, nextActorId);
    actorTable.insert(actor);
    std::cout << "[Success] Added new actor: \"" << trimmedName
        << "\" (ID=" << actor.getId() << ")\n";
    ++nextActorId;
}

/**
 * @brief Adds a new movie to the system.
 *
 * Validates input, ensures a unique movie ID, creates a Movie, and inserts it
 * into the movie hash table.
 *
 * @param title The title of the movie.
 * @param plot The plot of the movie.
 * @param releaseYear The release year of the movie.
 */
void MovieApp::addNewMovie(const std::string& title, const std::string& plot, int releaseYear) {
    if (!isAdmin) {
        std::cout << "[Error] Only administrators can add new movies.\n";
        return;
    }
    std::string trimmedTitle = trimQuotes(title);
    if (trimmedTitle.empty()) {
        std::cout << "[Error] Movie title cannot be empty.\n";
        return;
    }
    if (releaseYear <= 1887 || releaseYear > 2025) {
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

/**
 * @brief Links an actor to a movie using their IDs.
 *
 * Looks up the actor and movie in their respective hash tables and adds the actor
 * to the movie's cast.
 *
 * @param actorId The actor's ID.
 * @param movieId The movie's ID.
 * @param isAdmin Flag indicating whether the operation is performed in admin mode.
 */
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

/**
 * @brief Updates an actor's details.
 *
 * Looks up the actor by ID, validates the new details, and updates the actor in
 * both the actor table and in all movies containing that actor.
 *
 * @param actorId The actor's ID.
 * @param newName The new name.
 * @param newYearOfBirth The new birth year.
 */
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
    std::string trimmedName = trim(newName);
    if (trimmedName.empty()) {
        std::cout << "[Error] Actor name cannot be empty.\n";
        return;
    }
    if (newYearOfBirth <= 0 || newYearOfBirth > 2025) {
        std::cout << "[Error] Invalid year of birth.\n";
        return;
    }
    // Update the actor in the hash table.
    actor->setName(trimmedName.c_str());
    actor->setBirthYear(newYearOfBirth);
    actorTable.insert(*actor);

    // Update actor details in all movies.
    movieTable.forEach([&](const Movie& movieRef) -> bool {
        Movie& movie = const_cast<Movie&>(movieRef);
        if (movie.hasActor(actorId)) {
            movie.getActors().forEach([&](const Actor& castActor) -> bool {
                Actor& mutableActor = const_cast<Actor&>(castActor);
                if (mutableActor.getId() == actorId) {
                    mutableActor.setName(trimmedName.c_str());
                    mutableActor.setBirthYear(newYearOfBirth);
                }
                return false; // Continue iteration.
                });
        }
        return false;
        });
    std::cout << "[Success] Updated Actor ID " << actorId << "\n";
}

/**
 * @brief Updates a movie's details.
 *
 * Looks up the movie by ID, validates the new details, and updates the movie
 * in the movie table.
 *
 * @param movieId The movie's ID.
 * @param newTitle The new title.
 * @param newPlot The new plot.
 * @param newReleaseYear The new release year.
 */
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
    std::string trimmedTitle = trimQuotes(newTitle);
    if (trimmedTitle.empty()) {
        std::cout << "[Error] Movie title cannot be empty.\n";
        return;
    }
    if (newReleaseYear <= 1888 || newReleaseYear > 2025) {
        std::cout << "[Error] Invalid release year.\n";
        return;
    }
    movie->setTitle(trimmedTitle.c_str());
    movie->setPlot(newPlot.c_str());
    movie->setReleaseYear(newReleaseYear);
    movieTable.insert(*movie);
    std::cout << "[Success] Updated Movie ID " << movieId << "\n";
}

/**
 * @brief Finds actors by name.
 *
 * Iterates over all actors in the actor table and adds those that match the
 * provided name (case-insensitive) to the result list.
 *
 * @param name The actor name to search for.
 * @param result The list to populate with matching actors.
 */
void MovieApp::findActorsByName(const std::string& name, List<Actor>& result) const {
    actorTable.forEach([&](const Actor& a) -> bool {
        if (caseInsensitiveCompareStrings(a.getName(), name.c_str()) == 0) {
            result.add(a);
        }
        return false; // Continue iterating.
        });
}

// ---------------------------------------------------------------------------
// Display Methods with Sorting and Pagination
// ---------------------------------------------------------------------------

/**
 * @brief Merge sort helper for sorting Movie objects alphabetically by title.
 *
 * @param arr The array of Movie objects.
 * @param left The left index.
 * @param right The right index.
 */
static void mergeSortMovies(Movie* arr, int left, int right) {
    if (left >= right) return;
    int mid = (left + right) / 2;
    mergeSortMovies(arr, left, mid);
    mergeSortMovies(arr, mid + 1, right);
    int size = right - left + 1;
    Movie* temp = new Movie[size];
    int i = left, j = mid + 1, k = 0;
    while (i <= mid && j <= right) {
        if (caseInsensitiveCompareStrings(arr[i].getTitle(), arr[j].getTitle()) <= 0) {
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

/**
 * @brief Displays all movies sorted alphabetically by title with pagination.
 */
void MovieApp::displayAllMovies() const {
    if (movieTable.isEmpty()) {
        std::cout << "No movies found.\n";
        return;
    }
    int capacity = 100, count = 0;
    Movie* arr = new Movie[capacity];
    movieTable.forEach([&](const Movie& m) -> bool {
        if (count >= capacity) {
            int newCapacity = capacity * 2;
            Movie* newArr = new Movie[newCapacity];
            for (int i = 0; i < count; i++) {
                newArr[i] = arr[i];
            }
            delete[] arr;
            arr = newArr;
            capacity = newCapacity;
        }
        arr[count++] = m;
        return false;
        });
    if (count == 0) {
        std::cout << "No movies found.\n";
        delete[] arr;
        return;
    }
    mergeSortMovies(arr, 0, count - 1);
    std::cout << "All Movies (alphabetical):\n";
    const int pageSize = 8000; // Movies per page.
    int linesPrinted = 0;
    for (int i = 0; i < count; i++) {
        arr[i].displayDetails();
        linesPrinted++;
        if (linesPrinted % pageSize == 0 && i < count - 1) {
            std::cout << "\n-- Press Enter to continue --";
            std::cout.flush();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "\n";
        }
    }
    delete[] arr;
}

/**
 * @brief Merge sort helper for sorting Actor objects by age in ascending order.
 *
 * @param arr The array of Actor objects.
 * @param left The left index.
 * @param right The right index.
 */
static void mergeSortActorsByAge(Actor* arr, int left, int right) {
    if (left >= right) return;
    int mid = (left + right) / 2;
    mergeSortActorsByAge(arr, left, mid);
    mergeSortActorsByAge(arr, mid + 1, right);
    int size = right - left + 1;
    Actor* temp = new Actor[size];
    int i = left, j = mid + 1, k = 0;
    while (i <= mid && j <= right) {
        if (arr[i].getAge() <= arr[j].getAge()) {
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

/**
 * @brief Merge sort helper for sorting Actor objects alphabetically by name.
 *
 * @param arr The array of Actor objects.
 * @param left The left index.
 * @param right The right index.
 */
static void mergeSortActors(Actor* arr, int left, int right) {
    if (left >= right) return;
    int mid = (left + right) / 2;
    mergeSortActors(arr, left, mid);
    mergeSortActors(arr, mid + 1, right);
    int size = right - left + 1;
    Actor* temp = new Actor[size];
    int i = left, j = mid + 1, k = 0;
    while (i <= mid && j <= right) {
        if (caseInsensitiveCompareStrings(arr[i].getName(), arr[j].getName()) <= 0) {
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

/**
 * @brief Displays all actors sorted alphabetically by name with pagination.
 */
void MovieApp::displayAllActors() const {
    if (actorTable.isEmpty()) {
        std::cout << "No actors found.\n";
        return;
    }
    int capacity = 100, count = 0;
    Actor* arr = new Actor[capacity];
    actorTable.forEach([&](const Actor& a) -> bool {
        if (count >= capacity) {
            int newCapacity = capacity * 2;
            Actor* newArr = new Actor[newCapacity];
            for (int i = 0; i < count; i++) {
                newArr[i] = arr[i];
            }
            delete[] arr;
            arr = newArr;
            capacity = newCapacity;
        }
        arr[count++] = a;
        return false;
        });
    if (count == 0) {
        std::cout << "No actors found.\n";
        delete[] arr;
        return;
    }
    mergeSortActors(arr, 0, count - 1);
    std::cout << "All Actors (alphabetical):\n";
    const int pageSize = 8000;
    int linesPrinted = 0;
    for (int i = 0; i < count; i++) {
        arr[i].displayDetails();
        linesPrinted++;
        if (linesPrinted % pageSize == 0 && i < count - 1) {
            std::cout << "\n-- Press Enter to continue --";
            std::cout.flush();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "\n";
        }
    }
    delete[] arr;
}

/**
 * @brief Displays actors whose ages fall within a given range, sorted by age.
 *
 * @param minAge The minimum age.
 * @param maxAge The maximum age.
 */
void MovieApp::displayActorsByAge(int minAge, int maxAge) const {
    int capacity = 100, count = 0;
    Actor* actorsInRange = new Actor[capacity];
    actorTable.forEach([&](const Actor& a) -> bool {
        int age = a.getAge();
        if (age >= minAge && age <= maxAge) {
            if (count >= capacity) {
                int newCapacity = capacity * 2;
                Actor* newArr = new Actor[newCapacity];
                for (int i = 0; i < count; i++) {
                    newArr[i] = actorsInRange[i];
                }
                delete[] actorsInRange;
                actorsInRange = newArr;
                capacity = newCapacity;
            }
            actorsInRange[count++] = a;
        }
        return false;
        });
    if (count == 0) {
        std::cout << "No actors found in age range [" << minAge << ", " << maxAge << "].\n";
        delete[] actorsInRange;
        return;
    }
    mergeSortActorsByAge(actorsInRange, 0, count - 1);
    std::cout << "Actors in age range [" << minAge << ", " << maxAge << "] (sorted by age):\n";
    for (int i = 0; i < count; ++i) {
        std::cout << actorsInRange[i].getName() << " (Age=" << actorsInRange[i].getAge() << ")\n";
    }
    delete[] actorsInRange;
}

/**
 * @brief Merge sort helper for sorting Movie objects by release year.
 *
 * @param arr The array of Movie objects.
 * @param left The left index.
 * @param right The right index.
 */
static void mergeSortMoviesByYear(Movie* arr, int left, int right) {
    if (left >= right) return;
    int mid = (left + right) / 2;
    mergeSortMoviesByYear(arr, left, mid);
    mergeSortMoviesByYear(arr, mid + 1, right);
    int size = right - left + 1;
    Movie* temp = new Movie[size];
    int i = left, j = mid + 1, k = 0;
    while (i <= mid && j <= right) {
        if (arr[i].getReleaseYear() <= arr[j].getReleaseYear()) {
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

/**
 * @brief Displays recent movies (released within the last 3 years) sorted by release year.
 */
void MovieApp::displayRecentMovies() const {
    time_t now = time(0);
    struct tm localTime;
    localtime_s(&localTime, &now);
    int currentYear = localTime.tm_year + 1900;
    int cutoff = currentYear - 3;
    int capacity = 100, count = 0;
    Movie* recentMovies = new Movie[capacity];
    movieTable.forEach([&](const Movie& m) -> bool {
        if (m.getReleaseYear() >= cutoff) {
            if (count >= capacity) {
                int newCapacity = capacity * 2;
                Movie* newArr = new Movie[newCapacity];
                for (int i = 0; i < count; i++) {
                    newArr[i] = recentMovies[i];
                }
                delete[] recentMovies;
                recentMovies = newArr;
                capacity = newCapacity;
            }
            recentMovies[count++] = m;
        }
        return false;
        });
    if (count == 0) {
        std::cout << "No movies found in the last 3 years.\n";
        delete[] recentMovies;
        return;
    }
    mergeSortMoviesByYear(recentMovies, 0, count - 1);
    std::cout << "Recent Movies (in ascending order of release year):\n";
    for (int i = 0; i < count; ++i) {
        std::cout << recentMovies[i].getTitle()
            << " (" << recentMovies[i].getReleaseYear() << ")\n";
    }
    delete[] recentMovies;
}

/**
 * @brief Displays movies featuring a given actor.
 *
 * Searches for movies in which the actor (identified by actorId) appears, sorts
 * them alphabetically by title, and displays the results.
 *
 * @param actorId The actor's ID.
 */
void MovieApp::displayMoviesOfActor(int actorId) const {
    Actor* actPtr = actorTable.find(actorId);
    if (!actPtr) {
        std::cout << "Actor ID " << actorId << " not found.\n";
        return;
    }
    int capacity = 100, count = 0;
    Movie* arr = new Movie[capacity];
    movieTable.forEach([&](const Movie& m) -> bool {
        if (m.hasActor(actorId)) {
            if (count >= capacity) {
                int newCapacity = capacity * 2;
                Movie* newArr = new Movie[newCapacity];
                for (int i = 0; i < count; i++) {
                    newArr[i] = arr[i];
                }
                delete[] arr;
                arr = newArr;
                capacity = newCapacity;
            }
            arr[count++] = m;
        }
        return false;
        });
    if (count == 0) {
        std::cout << "No movies found for actor ID " << actorId << ".\n";
        delete[] arr;
        return;
    }
    mergeSortMovies(arr, 0, count - 1);
    std::cout << "Movies for actor ID " << actorId << ":\n";
    for (int i = 0; i < count; ++i) {
        std::cout << " - " << arr[i].getTitle()
            << " (" << arr[i].getReleaseYear() << ")\n";
    }
    delete[] arr;
}

/**
 * @brief Displays actors in a movie specified by title.
 *
 * Searches for a movie with a matching title (case-insensitive), sorts its cast
 * alphabetically by name, and displays the actor names and ages.
 *
 * @param movieTitle The title of the movie.
 */
void MovieApp::displayActorsInMovie(const std::string& movieTitle) const {
    bool foundMovie = false;
    movieTable.forEach([&](const Movie& m) -> bool {
        if (caseInsensitiveCompareStrings(m.getTitle(), movieTitle.c_str()) == 0) {
            foundMovie = true;
            int capacity = 100, count = 0;
            Actor* actorArr = new Actor[capacity];
            m.getActors().forEach([&](const Actor& a) -> bool {
                if (count >= capacity) {
                    int newCapacity = capacity * 2;
                    Actor* newArr = new Actor[newCapacity];
                    for (int i = 0; i < count; i++) {
                        newArr[i] = actorArr[i];
                    }
                    delete[] actorArr;
                    actorArr = newArr;
                    capacity = newCapacity;
                }
                actorArr[count++] = a;
                return false;
                });
            mergeSortActors(actorArr, 0, count - 1);
            std::cout << "Actors in \"" << movieTitle << "\":\n";
            for (int i = 0; i < count; ++i) {
                std::cout << " - " << actorArr[i].getName()
                    << " (Age: " << actorArr[i].getAge() << ")\n";
            }
            delete[] actorArr;
            return true; // Stop iteration after matching movie is found.
        }
        return false;
        });
    if (!foundMovie) {
        std::cout << "Movie \"" << movieTitle << "\" not found.\n";
    }
}

/**
 * @brief Displays actors known by a given actor (up to 2 levels).
 *
 * Finds the actor by name, builds the actor graph, and uses breadth-first search
 * to determine which actors are connected within 2 levels.
 *
 * @param actorName The name of the starting actor.
 */
void MovieApp::displayActorsKnownBy(const std::string& actorName) const {
    int startActorId = -1;
    actorTable.forEach([&](const Actor& a) -> bool {
        if (caseInsensitiveCompareStrings(a.getName(), actorName.c_str()) == 0) {
            startActorId = a.getId();
            return true; // Stop once found.
        }
        return false;
        });
    if (startActorId == -1) {
        std::cout << "[Error] Actor \"" << actorName << "\" not found.\n";
        return;
    }
    int* actorIds = nullptr;
    List<int>* adjacencyLists = nullptr;
    int totalActors = 0;
    ActorGraph::buildActorGraph(actorTable, movieTable, actorIds, totalActors, adjacencyLists);
    int startIndex = ActorGraph::findActorIndexInArray(startActorId, actorIds, totalActors);
    if (startIndex == -1) {
        std::cout << "[Error] Could not map actor to index.\n";
        delete[] adjacencyLists;
        delete[] actorIds;
        return;
    }
    List<int> discovered = ActorGraph::findConnectedActors(startIndex, adjacencyLists, totalActors, 2);
    if (discovered.isEmpty()) {
        std::cout << "No actors found that \"" << actorName << "\" knows (up to 2 levels).\n";
        delete[] adjacencyLists;
        delete[] actorIds;
        return;
    }
    std::cout << "Actors known by \"" << actorName << "\" (up to 2 levels):\n";
    discovered.forEach([&](int idx) {
        if (idx < 0 || idx >= totalActors) return false;
        int realActorId = actorIds[idx];
        Actor* aPtr = actorTable.find(realActorId);
        if (aPtr) {
            std::cout << " - " << aPtr->getName() << "\n";
        }
        return false;
        });
    delete[] adjacencyLists;
    delete[] actorIds;
}

// ---------------------------------------------------------------------------
// Rating and Recommendation Methods
// ---------------------------------------------------------------------------

/**
 * @brief Sets the rating for an actor.
 *
 * Validates the rating value, updates the actor's rating, and re-inserts the
 * updated actor into the hash table.
 *
 * @param actorId The actor's ID.
 * @param rating The new rating (1-10).
 */
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
    actorTable.insert(*a);
    std::cout << "[Success] Actor (ID=" << actorId << ") rating updated to " << rating << "\n";
}

/**
 * @brief Sets the rating for a movie.
 *
 * Validates the rating value, updates the movie's rating, and re-inserts the
 * updated movie into the hash table.
 *
 * @param movieId The movie's ID.
 * @param rating The new rating (1-10).
 */
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

/**
 * @brief Recommends movies within a specified rating range.
 *
 * Displays movies from the movieRatings hash table that have ratings between
 * minRating and maxRating.
 *
 * @param minRating The minimum rating.
 * @param maxRating The maximum rating.
 */
void MovieApp::recommendMoviesByRating(int minRating, int maxRating) const {
    std::cout << "Movies with rating between " << minRating << " and " << maxRating << ":\n";
    movieRatings.displayMoviesInRange(minRating, maxRating);
}

/**
 * @brief Recommends actors within a specified rating range.
 *
 * Displays actors from the actorRatings hash table that have ratings between
 * minRating and maxRating.
 *
 * @param minRating The minimum rating.
 * @param maxRating The maximum rating.
 */
void MovieApp::recommendActorsByRating(int minRating, int maxRating) const {
    std::cout << "Actors with rating between " << minRating << " and " << maxRating << ":\n";
    actorRatings.displayActorsInRange(minRating, maxRating);
}

