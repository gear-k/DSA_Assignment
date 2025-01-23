#include "MovieApp.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>   // for atoi
#include <cstring>   // for strcmp
#include "ActorGraph.h"

using namespace std;

// Helper to trim quotes/spaces
static std::string trimQuotes(const std::string& str) {
    size_t start = str.find_first_not_of(" \"");
    size_t end = str.find_last_not_of(" \"");
    if (start == std::string::npos || end == std::string::npos) {
        return "";
    }
    return str.substr(start, end - start + 1);
}



// Helper function to trim leading and trailing spaces
static std::string trim(const std::string& str) {
    size_t start = str.find_first_not_of(" \t");
    size_t end = str.find_last_not_of(" \t");

    if (start == std::string::npos || end == std::string::npos) {
        return ""; // String is empty or contains only spaces
    }

    return str.substr(start, end - start + 1);
}


MovieApp::MovieApp()
    : nextActorId(1000), nextMovieId(5000) , movieRatings(), actorRatings() {
}

//------------------------------------------------------------------------------
void MovieApp::readActors(const std::string& filename) {
    ifstream fin(filename.c_str());
    if (!fin.is_open()) {
        cerr << "[Error] Could not open file: " << filename << "\n";
        return;
    }

    string header;
    if (!getline(fin, header)) {
        cerr << "[Error] File is empty: " << filename << "\n";
        fin.close();
        return;
    }

    string line;
    while (getline(fin, line)) {
        if (line.empty()) continue;

        stringstream ss(line);
        string idStr, nameStr, birthStr;

        getline(ss, idStr, ',');
        getline(ss, nameStr, ',');
        getline(ss, birthStr, ',');

        if (idStr.empty() || nameStr.empty() || birthStr.empty()) {
            cerr << "[Warning] Malformed row in " << filename << ": " << line << "\n";
            continue;
        }

        int id = atoi(idStr.c_str());
        int birth = atoi(birthStr.c_str());
        nameStr = trimQuotes(nameStr);

        Actor actor(nameStr.c_str(), birth, id);
        actorList.add(actor);
        /*
        if (id >= nextActorId) {
            nextActorId = id + 1;
        }
        */
    }
    fin.close();
}

void MovieApp::readMovies(const std::string& filename) {
    ifstream fin(filename.c_str());
    if (!fin.is_open()) {
        cerr << "[Error] Could not open file: " << filename << "\n";
        return;
    }

    string header;
    if (!getline(fin, header)) {
        cerr << "[Error] File is empty: " << filename << "\n";
        fin.close();
        return;
    }

    string line;
    while (getline(fin, line)) {
        if (line.empty()) continue;

        stringstream ss(line);
        string idStr, titleStr, yearStr;

        getline(ss, idStr, ',');
        getline(ss, titleStr, ',');
        getline(ss, yearStr, ',');

        if (idStr.empty() || titleStr.empty() || yearStr.empty()) {
            cerr << "[Warning] Malformed row in " << filename << ": " << line << "\n";
            continue;
        }

        int id = atoi(idStr.c_str());
        int year = atoi(yearStr.c_str());
        titleStr = trimQuotes(titleStr);

        Movie movie(titleStr.c_str(), "", year, id);
        movieList.add(movie);
        /*
        if (id >= nextMovieId) {
            nextMovieId = id + 1;
        }
        */
    }
    fin.close();
}

bool MovieApp::isActorIdUsed(int id) const {
    bool used = false;
    actorList.display([&](const Actor& a) {
        if (a.getId() == id) {
            used = true;
            return true; // Stop traversal
        }
        return false;
        });
    return used;
}

bool MovieApp::isMovieIdUsed(int id) const {
    bool used = false;
    movieList.display([&](const Movie& m) {
        if (m.getId() == id) {
            used = true;
            return true; // Stop traversal
        }
        return false;
        });
    return used;
}



void MovieApp::readCast(const std::string& filename) {
    ifstream fin(filename.c_str());
    if (!fin.is_open()) {
        cerr << "[Error] Could not open file: " << filename << "\n";
        return;
    }

    string header;
    if (!getline(fin, header)) {
        cerr << "[Error] File is empty: " << filename << "\n";
        fin.close();
        return;
    }

    string line;
    while (getline(fin, line)) {
        if (line.empty()) continue;

        stringstream ss(line);
        string pidStr, midStr;

        getline(ss, pidStr, ',');
        getline(ss, midStr, ',');

        if (pidStr.empty() || midStr.empty()) {
            cerr << "[Warning] Malformed row in " << filename << ": " << line << "\n";
            continue;
        }

        int personId = atoi(pidStr.c_str());
        int movieId = atoi(midStr.c_str());

        Actor* foundActor = nullptr;
        actorList.display([&](Actor& a) {
            if (a.getId() == personId) {
                foundActor = &a;
                return true;
            }
            return false;
            });

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
// Add New Actor with Improved Validation
//------------------------------------------------------------------------------
void MovieApp::addNewActor(const std::string& name, int birthYear) {
    std::string trimmedName = trim(name);

    // Validate name
    if (trimmedName.empty()) {
        cout << "[Error] Actor name cannot be empty or just spaces. Please try again.\n";
        return;
    }

    // Validate year of birth
    if (birthYear <= 0 || birthYear >= 2025) {
        cout << "[Error] Invalid birth year. Must be greater than 0 and less than 2025.\n";
        return;
    }

    // Find the next available Actor ID starting from 1000
    while (isActorIdUsed(nextActorId)) {
        nextActorId++;
    }

    // Add actor if valid
    Actor actor(trimmedName.c_str(), birthYear, nextActorId);
    cout << "[Success] Added new actor: \"" << trimmedName << "\" (ID=" << actor.getId() << ")\n";
    actorList.add(actor);
    ++nextActorId;
}




void MovieApp::addNewMovie(const std::string& title, const std::string& plot, int releaseYear) {
    std::string trimmedTitle = trim(title);

    // Validate title
    if (trimmedTitle.empty()) {
        cout << "[Error] Movie title cannot be empty or just spaces. Please try again.\n";
        return;
    }

    // Validate release year
    if (releaseYear <= 1800 || releaseYear >= 2025) {
        cout << "[Error] Invalid release year. Must be greater than 1800 and less than 2025.\n";
        return;
    }

    // Find the next available Movie ID starting from 5000
    while (isMovieIdUsed(nextMovieId)) {
        nextMovieId++;
    }

    // Add movie if valid
    Movie movie(trimmedTitle.c_str(), plot.c_str(), releaseYear, nextMovieId);
    cout << "[Success] Added new movie: \"" << trimmedTitle << "\" (ID=" << movie.getId() << ")\n";
    movieList.add(movie);
    ++nextMovieId;
}




//------------------------------------------------------------------------------
// 3) Link actors & movies
//------------------------------------------------------------------------------

void MovieApp::addActorToMovieById(int actorId, int movieId) {
    // 1) Find the Actor
    Actor* targetActor = nullptr;
    actorList.display([&](Actor& a) {
        if (a.getId() == actorId) {
            targetActor = &a;
            return true; // Found
        }
        return false;
        });
    if (!targetActor) {
        cout << "[Error] Actor ID " << actorId << " not found.\n";
        return;
    }

    // 2) Find the Movie
    Movie* targetMovie = nullptr;
    movieList.display([&](Movie& m) {
        if (m.getId() == movieId) {
            targetMovie = &m;
            return true; // Found
        }
        return false;
        });
    if (!targetMovie) {
        cout << "[Error] Movie ID " << movieId << " not found.\n";
        return;
    }

    // 3) Link them
    targetMovie->addActor(*targetActor);
    cout << "Actor \"" << targetActor->getName()
        << "\" added to movie \"" << targetMovie->getTitle() << "\"\n";
}


//------------------------------------------------------------------------------
// 4) Update actor/movie
//------------------------------------------------------------------------------

// Updated updateActorDetails with input validation
void MovieApp::updateActorDetails(int actorId, const std::string& newName, int newYearOfBirth) {
    bool found = false;
    actorList.display([&](Actor& a) {
        if (a.getId() == actorId) {
            if (newName.empty()) {
                cout << "[Error] New actor name cannot be empty.\n";
                found = true; // Prevents "not found" message
                return true;
            }
            if (newYearOfBirth <= 0 || newYearOfBirth >= 2025) {
                cout << "[Error] Invalid year of birth. Must be > 0 and < 2025.\n";
                found = true;
                return true;
            }
            a.setName(newName.c_str());
            a.setBirthYear(newYearOfBirth);
            cout << "Updated Actor ID " << actorId
                << " => Name: " << newName
                << ", Birth Year: " << newYearOfBirth << endl;
            found = true;
            return true;
        }
        return false;
        });
    if (!found) {
        cout << "[Error] Actor ID " << actorId << " not found.\n";
    }
}

// Updated updateMovieDetails with input validation
void MovieApp::updateMovieDetails(int movieId, const std::string& newTitle, const std::string& newPlot, int newReleaseYear) {
    bool found = false;
    movieList.display([&](Movie& m) {
        if (m.getId() == movieId) {
            if (newTitle.empty()) {
                cout << "[Error] New movie title cannot be empty.\n";
                found = true; // Prevents "not found" message
                return true;
            }
            if (newReleaseYear <= 1800 || newReleaseYear >= 2025) {
                cout << "[Error] Invalid release year. Must be > 1800 and < 2025.\n";
                found = true;
                return true;
            }
            m.setTitle(newTitle.c_str());
            m.setPlot(newPlot.c_str());
            m.setReleaseYear(newReleaseYear);
            cout << "Updated Movie ID " << movieId
                << " => Title: " << newTitle
                << ", Plot: " << newPlot
                << ", Release Year: " << newReleaseYear << endl;
            found = true;
            return true;
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

// Merge sort for Movie array
void mergeSortMovies(Movie* arr, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;

        // Sort first and second halves
        mergeSortMovies(arr, left, mid);
        mergeSortMovies(arr, mid + 1, right);

        // Merge the sorted halves
        Movie* temp = new Movie[right - left + 1];
        int i = left, j = mid + 1, k = 0;

        while (i <= mid && j <= right) {
            if (strcmp(arr[i].getTitle(), arr[j].getTitle()) <= 0) {
                temp[k++] = arr[i++];
            }
            else {
                temp[k++] = arr[j++];
            }
        }

        // Copy remaining elements
        while (i <= mid) temp[k++] = arr[i++];
        while (j <= right) temp[k++] = arr[j++];

        // Copy sorted elements back into original array
        for (int i = 0; i < k; ++i) {
            arr[left + i] = temp[i];
        }

        delete[] temp;
    }
}

// Merge sort for Actor array
void mergeSortActors(Actor* arr, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;

        // Sort first and second halves
        mergeSortActors(arr, left, mid);
        mergeSortActors(arr, mid + 1, right);

        // Merge the sorted halves
        Actor* temp = new Actor[right - left + 1];
        int i = left, j = mid + 1, k = 0;

        while (i <= mid && j <= right) {
            if (strcmp(arr[i].getName(), arr[j].getName()) <= 0) {
                temp[k++] = arr[i++];
            }
            else {
                temp[k++] = arr[j++];
            }
        }

        // Copy remaining elements
        while (i <= mid) temp[k++] = arr[i++];
        while (j <= right) temp[k++] = arr[j++];

        // Copy sorted elements back into original array
        for (int i = 0; i < k; ++i) {
            arr[left + i] = temp[i];
        }

        delete[] temp;
    }
}

void MovieApp::displayAllMovies() const {
    if (movieList.isEmpty()) {
        std::cout << "No movies found.\n";
        return;
    }

    static const int MAX_MOVIES = 2000;
    Movie* movieArray = new Movie[MAX_MOVIES];
    int count = 0;

    // Collect movies into an array
    movieList.display([&](const Movie& movie) {
        if (count < MAX_MOVIES) {
            movieArray[count++] = movie;
        }
        return false;
        });

    if (count == 0) {
        std::cout << "No movies found.\n";
        delete[] movieArray;
        return;
    }

    // Use merge sort to sort movies alphabetically by title
    mergeSortMovies(movieArray, 0, count - 1);

    // Display sorted movies
    std::cout << "All Movies (alphabetical order):\n";
    for (int i = 0; i < count; ++i) {
        movieArray[i].displayDetails();
    }

    delete[] movieArray;
}

void MovieApp::displayAllActors() const {
    if (actorList.isEmpty()) {
        std::cout << "No actors found.\n";
        return;
    }

    static const int MAX_ACTORS = 2000;
    Actor* actorArray = new Actor[MAX_ACTORS];
    int count = 0;

    // Collect actors into an array
    actorList.display([&](const Actor& actor) {
        if (count < MAX_ACTORS) {
            actorArray[count++] = actor;
        }
        return false;
        });

    if (count == 0) {
        std::cout << "No actors found.\n";
        delete[] actorArray;
        return;
    }

    // Use merge sort to sort actors alphabetically by name
    mergeSortActors(actorArray, 0, count - 1);

    // Display sorted actors
    std::cout << "All Actors (alphabetical order):\n";
    for (int i = 0; i < count; ++i) {
        actorArray[i].displayDetails();
    }

    delete[] actorArray;
}


void MovieApp::displayActorsByAge(int minAge, int maxAge) const {
    if (actorList.isEmpty()) {
        cout << "No actors found.\n";
        return;
    }

    bool anyFound = false;

    // Display actors whose ages fall within the range [minAge, maxAge]
    actorList.display([&](const Actor& a) {
        int age = a.getAge();
        if (age >= minAge && age <= maxAge) {
            cout << a.getName() << " (Age=" << age << ")\n";
            anyFound = true;
        }
        return false; // Continue traversal
        });

    if (!anyFound) {
        cout << "No actors found in the age range [" << minAge << ", " << maxAge << "].\n";
    }
}


void MovieApp::displayRecentMovies() const {
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
    static const int MAX_MOVIES = 2000;
    // Allocate on the heap
    Movie* foundMovies = new Movie[MAX_MOVIES];
    int foundCount = 0;

    // Collect all movies that have this actor
    movieList.display([&](const Movie& m) {
        if (m.hasActor(actorName.c_str())) {
            if (foundCount < MAX_MOVIES) {
                foundMovies[foundCount++] = m;
            }
        }
        return false;
        });

    if (foundCount == 0) {
        cout << "No movies found for actor \"" << actorName << "\".\n";
        delete[] foundMovies; // Clean up
        return;
    }

    // Bubble Sort to sort movies by title
    for (int i = 0; i < foundCount - 1; ++i) {
        for (int j = 0; j < foundCount - i - 1; ++j) {
            if (strcmp(foundMovies[j].getTitle(), foundMovies[j + 1].getTitle()) > 0) {
                std::swap(foundMovies[j], foundMovies[j + 1]);
            }
        }
    }

    // Display sorted movies
    cout << "Movies for actor \"" << actorName << "\" (alphabetical order):\n";
    for (int i = 0; i < foundCount; ++i) {
        cout << " - " << foundMovies[i].getTitle()
            << " (" << foundMovies[i].getReleaseYear() << ")\n";
    }

    // Clean up heap memory
    delete[] foundMovies;
}



void MovieApp::displayActorsInMovie(const std::string& movieTitle) const {
    bool foundMovie = false;

    // Find the movie, then collect its actors
    movieList.display([&](const Movie& m) {
        if (strcmp(m.getTitle(), movieTitle.c_str()) == 0) {
            foundMovie = true;

            static const int MAX_ACTORS = 2000;
            Actor actorArray[MAX_ACTORS]; // Ensure Actor has a proper copy constructor
            int count = 0;

            // Collect all actors from the movie
            m.getActors().display([&](const Actor& a) {
                if (count < MAX_ACTORS) {
                    actorArray[count++] = a; // Ensure proper assignment
                }
                else {
                    std::cerr << "[Error] Too many actors in the movie \"" << movieTitle << "\".\n";
                }
                return false;
                });

            // Bubble Sort to sort actors by name
            for (int i = 0; i < count - 1; ++i) {
                for (int j = 0; j < count - i - 1; ++j) {
                    if (strcmp(actorArray[j].getName(), actorArray[j + 1].getName()) > 0) {
                        std::swap(actorArray[j], actorArray[j + 1]);
                    }
                }
            }

            // Display sorted actors
            cout << "Actors in \"" << movieTitle << "\" (alphabetical order):\n";
            for (int i = 0; i < count; ++i) {
                cout << " - " << actorArray[i].getName()
                    << " (Age: " << actorArray[i].getAge() << ")\n";
            }
            return true; // We found the movie, so stop the traversal
        }
        return false;
        });

    if (!foundMovie) {
        cout << "Movie \"" << movieTitle << "\" not found.\n";
    }
}


// THIS IS A TEST MAKE SURE TO REMOVE THIS BEFORE SUBMISSION CEDRICCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
void MovieApp::runAllTests() {
    cout << "\n========== Running All Tests ==========\n";

    // **Reset IDs for testing purposes**
    nextActorId = 1000;
    nextMovieId = 5000;

    // **Clear existing data to start fresh**
    actorList.clear();
    movieList.clear();

    // 1. Test Adding New Actors
    cout << "\n-- Test 1: Adding New Actors --\n";
    // Valid additions
    addNewActor("Leonardo DiCaprio", 1974);       // ID=1000
    addNewActor("Kate Winslet", 1975);            // ID=1001
    addNewActor("Brad Pitt", 1963);               // ID=1002
    addNewActor("Morgan Freeman", 1937);          // ID=1003
    addNewActor("Scarlett Johansson", 1984);      // ID=1004
    // Invalid additions
    addNewActor("", 1980);                        // Invalid name
    addNewActor("Tom Hanks", -5);                 // Invalid birth year
    addNewActor(" ", 1990);                       // Invalid name
    addNewActor("Morgan Freeman", 2026);          // Invalid birth year
    displayAllActors();

    // 2. Test Adding New Movies
    cout << "\n-- Test 2: Adding New Movies --\n";
    // Valid additions
    addNewMovie("Inception", "A thief who steals corporate secrets through dream-sharing technology.", 2010);          // ID=5000
    addNewMovie("Titanic", "A love story that unfolds aboard the ill-fated RMS Titanic.", 1997);                         // ID=5001
    addNewMovie("Fight Club", "An insomniac office worker and a soap maker form an underground fight club.", 1999);        // ID=5002
    addNewMovie("Avengers: Endgame", "The Avengers assemble once more to save the universe.", 2024);                        // ID=5003
    addNewMovie("The Shawshank Redemption", "Two imprisoned men bond over a number of years.", 1994);                      // ID=5004
    // Invalid additions
    addNewMovie("", "No plot", 2000);                                                    // Invalid title
    addNewMovie("The Matrix", "A computer hacker learns about the true nature of his reality.", 1700); // Invalid year
    addNewMovie(" ", "Plot", 2015);                                                      // Invalid title
    addNewMovie("Future Movie", "A movie from the future.", 2027);                        // Invalid year
    displayAllMovies();

    // 4. Test Updating Actor Details
    cout << "\n-- Test 4: Updating Actor Details --\n";
    // Assume actor IDs start at 1000 based on your constructor
    // Valid updates
    updateActorDetails(1000, "Leonardo Wilhelm DiCaprio", 1974);
    updateActorDetails(1001, "Katherine Winslet", 1975);
    updateActorDetails(1002, "Bradley Pitt", 1963); // Corrected name
    updateActorDetails(1003, "Morgan Freeman", 1937); // Valid update
    // Invalid updates
    updateActorDetails(9999, "Nonexistent Actor", 1980);          // Non-existent actor
    updateActorDetails(1002, "", 1963);                           // Invalid name
    updateActorDetails(1002, "Brad Pitt", -10);                   // Invalid year
    updateActorDetails(1004, "Scarlett Johansson", 2026);         // Invalid birth year
    displayAllActors();

    // 5. Test Updating Movie Details
    cout << "\n-- Test 5: Updating Movie Details --\n";
    // Assume movie IDs start at 5000 based on your constructor
    // Valid updates
    updateMovieDetails(5000, "Inception (Edited)", "A skilled thief is offered a chance to erase his past.", 2010);
    updateMovieDetails(5001, "Titanic (Edited)", "An epic romance unfolds aboard the Titanic.", 1997);
    updateMovieDetails(5002, "Fight Club (Edited)", "An underground fight club becomes something much more.", 1999);
    updateMovieDetails(5003, "Avengers: Endgame (Edited)", "The Avengers save the universe once more.", 2024); // Valid update
    updateMovieDetails(5004, "The Shawshank Redemption (Edited)", "Two imprisoned men bond over years.", 1994); // Valid update
    // Invalid updates
    updateMovieDetails(99999, "Unknown Movie", "No plot", 2020); // Non-existent movie
    updateMovieDetails(5002, "", "New plot", 2000);             // Invalid title
    updateMovieDetails(5002, "Fight Club (Edited Again)", "Updated plot", 1700); // Invalid year
    displayAllMovies();

    // 3. Test Adding Actors to Movies
    cout << "\n-- Test 3: Adding Actors to Movies --\n";
    // Valid associations using IDs
    addActorToMovieById(1000, 5000); // Leonardo DiCaprio to Inception (Edited)
    addActorToMovieById(1001, 5001); // Katherine Winslet to Titanic (Edited)
    addActorToMovieById(1002, 5002); // Bradley Pitt to Fight Club (Edited)
    addActorToMovieById(1000, 5001); // Leonardo DiCaprio to Titanic (Edited) as well
    addActorToMovieById(1004, 5003); // Scarlett Johansson to Avengers: Endgame (Edited)
    addActorToMovieById(1003, 5004); // Morgan Freeman to The Shawshank Redemption (Edited)
    addActorToMovieById(1002, 5004); // Bradley Pitt to The Shawshank Redemption (Edited)
    addActorToMovieById(1000, 5004); // Leonardo DiCaprio to The Shawshank Redemption (Edited)
    // Invalid associations using IDs
    addActorToMovieById(9999, 5000); // Tom Hanks does not exist
    addActorToMovieById(1000, 9999); // Unknown Movie does not exist
    addActorToMovieById(1003, 5002); // Morgan Freeman to Fight Club (Edited)
    addActorToMovieById(1000, 5002); // Leonardo DiCaprio to Fight Club (Edited)
    displayActorsInMovie("Inception (Edited)");
    displayActorsInMovie("Titanic (Edited)");
    displayActorsInMovie("Fight Club (Edited)");
    displayActorsInMovie("Avengers: Endgame (Edited)");
    displayActorsInMovie("The Shawshank Redemption (Edited)");
    displayActorsInMovie("Unknown Movie"); // Should show error

    // 6. Test Displaying All Actors
    cout << "\n-- Test 6: Displaying All Actors --\n";
    displayAllActors();

    // 7. Test Displaying All Movies
    cout << "\n-- Test 7: Displaying All Movies --\n";
    displayAllMovies();

    // 8. Test Displaying Actors by Age Range
    cout << "\n-- Test 8: Displaying Actors by Age Range (40-60) --\n";
    displayActorsByAge(40, 60);
    // Edge case tests
    cout << "\n-- Test 8.1: Displaying Actors by Age Range (0-100) --\n";
    displayActorsByAge(0, 100);   // Full range
    cout << "\n-- Test 8.2: Displaying Actors by Age Range (70-60) --\n";
    displayActorsByAge(70, 60);   // Invalid range (maxAge < minAge)
    cout << "\n-- Test 8.3: Displaying Actors by Age Range (25-30) --\n";
    displayActorsByAge(25, 30);   // Range with no actors
    cout << "\n-- Test 8.4: Displaying Actors by Age Range (100-150) --\n";
    displayActorsByAge(100, 150); // High range with no results

    // 9. Test Displaying Recent Movies (Released in Last 3 Years)
    cout << "\n-- Test 9: Displaying Recent Movies (Released in Last 3 Years) --\n";
    displayRecentMovies();
    // Add a recent movie to ensure functionality
    addNewMovie("New Blockbuster", "A newly released blockbuster movie.", 2024); // ID=5005
    addActorToMovieById(1000, 5005); // Leonardo Wilhelm DiCaprio to New Blockbuster
    addActorToMovieById(1001, 5005); // Katherine Winslet to New Blockbuster
    displayRecentMovies();
    // Add an invalid recent movie
    addNewMovie("Future Movie", "A movie from the future.", 2027); // Invalid year
    displayRecentMovies();

    // 10. Test Displaying Movies of an Actor
    cout << "\n-- Test 10: Displaying Movies of an Actor --\n";
    displayMoviesOfActor("Leonardo Wilhelm DiCaprio");
    displayMoviesOfActor("Katherine Winslet");
    displayMoviesOfActor("Bradley Pitt");
    displayMoviesOfActor("Scarlett Johansson");
    displayMoviesOfActor("Morgan Freeman");
    displayMoviesOfActor("Tom Hanks"); // Non-existent actor

    // 11. Test Displaying Actors in a Movie
    cout << "\n-- Test 11: Displaying Actors in a Movie --\n";
    displayActorsInMovie("Inception (Edited)");
    displayActorsInMovie("Titanic (Edited)");
    displayActorsInMovie("Fight Club (Edited)");
    displayActorsInMovie("Avengers: Endgame (Edited)");
    displayActorsInMovie("The Shawshank Redemption (Edited)");
    displayActorsInMovie("Unknown Movie"); // Should show error

    // 12. Test Displaying Actors Known by an Actor
    cout << "\n-- Test 12: Displaying Actors Known by an Actor --\n";
    displayActorsKnownBy("Leonardo Wilhelm DiCaprio");
    displayActorsKnownBy("Katherine Winslet");
    displayActorsKnownBy("Bradley Pitt");
    displayActorsKnownBy("Scarlett Johansson");
    displayActorsKnownBy("Morgan Freeman");
    displayActorsKnownBy("Tom Hanks"); // Non-existent actor
    displayActorsKnownBy("");          // Invalid actor name

    // 15. Test Setting Actor Rating
    cout << "\n-- Test 15: Setting Actor Ratings --\n";
    // Valid ratings
    setActorRating(1000, 9); // Leonardo
    setActorRating(1001, 8); // Katherine
    setActorRating(1002, 7); // Bradley
    setActorRating(1003, 10); // Morgan
    setActorRating(1004, 6); // Scarlett
    // Invalid ratings
    setActorRating(9999, 5); // Non-existent actor
    setActorRating(1000, 11); // Invalid rating
    setActorRating(1001, 0); // Invalid rating
    setActorRating(1001, -2); // Invalid rating
    displayAllActors();

    // 16. Test Setting Movie Rating
    cout << "\n-- Test 16: Setting Movie Ratings --\n";
    // Valid ratings
    setMovieRating(5000, 9); // Inception (Edited)
    setMovieRating(5001, 8); // Titanic (Edited)
    setMovieRating(5002, 7); // Fight Club (Edited)
    setMovieRating(5003, 10); // Avengers: Endgame (Edited)
    setMovieRating(5004, 6); // The Shawshank Redemption (Edited)
    setMovieRating(5005, 8); // New Blockbuster
    // Invalid ratings
    setMovieRating(99999, 5); // Non-existent movie
    setMovieRating(5000, 11); // Invalid rating
    setMovieRating(5001, 0); // Invalid rating
    displayAllMovies();

    // 17. Test Recommend Movies by Rating
    cout << "\n-- Test 17: Recommend Movies by Rating --\n";
    // Assuming a recommendMoviesByRating() function exists
    cout << "Top Rated Movies (Threshold: 8):\n";
    recommendMoviesByRating(8,10); // Display movies with rating >= 8

    // 18. Test Recommend Actors by Rating
    cout << "\n-- Test 18: Recommend Actors by Rating --\n";
    // Assuming a recommendActorsByRating() function exists
    cout << "Top Rated Actors (Threshold: 8):\n";
    recommendActorsByRating(8,10); // Display actors with rating >= 8

    cout << "\n========== All Tests Completed ==========\n";



}




//------------------------------------------------------------------------------
// BFS Utility (no STL) for displayActorsKnownBy
//------------------------------------------------------------------------------

// We map each actor's ID to an array index. Then adjacencyLists[i] is a list of indices
void MovieApp::buildActorGraph(
    int actorIds[],
    int& actorCount,
    List<int>* adjacencyLists,
    const int MAX_ACTORS
) const {
    actorCount = 0;

    // Step 1: Collect all actor IDs in actorIds array
    actorList.display([&](const Actor& actor) {
        if (actorCount < MAX_ACTORS) {
            actorIds[actorCount] = actor.getId();
            actorCount++;
        }
        return false;
        });

    // Step 2: Dynamically allocate temporary array for each movie
    movieList.display([&](const Movie& mov) {
        int* tmpIdx = new int[300]; // Dynamically allocate memory
        int tmpCount = 0;

        mov.getActors().display([&](const Actor& a) {
            int aIndex = findActorIndexInArray(a.getId(), actorIds, actorCount);
            if (aIndex != -1 && tmpCount < 300) {
                tmpIdx[tmpCount++] = aIndex;
            }
            return false;
            });

        for (int i = 0; i < tmpCount; i++) {
            for (int j = i + 1; j < tmpCount; j++) {
                adjacencyLists[tmpIdx[i]].add(tmpIdx[j]);
                adjacencyLists[tmpIdx[j]].add(tmpIdx[i]);
            }
        }

        delete[] tmpIdx; // Free dynamically allocated memory
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

// Custom minimal queue for BFS (stores up to 2000 entries)
struct BFSQueue {
    struct Pair {
        int idx;
        int depth;
    };
    Pair data[2000];
    int front;
    int rear;
    int count;

    BFSQueue() : front(0), rear(-1), count(0) {
        for (int i = 0; i < 2000; i++) {
            data[i].idx = 0;    // Initialize the idx field of each Pair
            data[i].depth = 0;  // Initialize the depth field of each Pair
        }
    }


    bool isEmpty() const { return count == 0; }
    bool isFull()  const { return count == 2000; }

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

void MovieApp::displayActorsKnownBy(const std::string& actorName) const {
    // 1) Find the starting actor ID
    int startActorId = -1;
    actorList.display([&](const Actor& a) {
        if (strcmp(a.getName(), actorName.c_str()) == 0) {
            startActorId = a.getId();
            return true;
        }
        return false;
        });

    if (startActorId == -1) {
        cout << "[Error] Actor \"" << actorName << "\" not found.\n";
        return;
    }

    // 2) Build adjacency lists using ActorGraph utility
    int actorIds[ActorGraph::MAX_ACTORS];
    List<int> adjacencyLists[ActorGraph::MAX_ACTORS];
    int totalActors = 0;

    ActorGraph::buildActorGraph(actorList, movieList, actorIds, totalActors, adjacencyLists);

    // 3) Find starting index
    int startIndex = ActorGraph::findActorIndexInArray(startActorId, actorIds, totalActors);
    if (startIndex == -1) {
        cout << "[Error] Actor index not found.\n";
        return;
    }

    // 4) Find connected actors using BFS
    List<int> discoveredIndices = ActorGraph::findConnectedActors(startIndex, adjacencyLists);

    if (discoveredIndices.isEmpty()) {
        cout << "No actors found that \"" << actorName << "\" knows (up to 2 levels).\n";
        return;
    }

    // 5) Display results
    cout << "Actors known by \"" << actorName << "\" (up to 2 levels):\n";

    discoveredIndices.display([&](int idx) {
        if (idx == startIndex) return false;
        int realActorId = actorIds[idx];

        actorList.display([&](const Actor& a) {
            if (a.getId() == realActorId) {
                cout << " - " << a.getName() << endl;
                return true;
            }
            return false;
            });
        return false;
        });
}

int MovieApp::getNextActorId() const {
    return nextActorId;
}

int MovieApp::getNextMovieId() const {
    return nextMovieId;
}


void MovieApp::setActorRating(int actorId, int rating) {
    // Ensure the rating is between 1 and 10
    if (rating < 1 || rating > 10) {
        cout << "[Error] Invalid rating value. Must be between 1 and 10.\n";
        return;
    }

    bool found = false;
    actorList.display([&](Actor& a) {
        if (a.getId() == actorId) {
            a.setRating(rating);      // Update the actor's rating
            actorRatings.insert(a);  // Insert the actor into the BST
            cout << "[Success] Actor (ID=" << actorId << ") rating updated to " << rating << "\n";
            found = true;
            return true; // Stop traversal
        }
        return false;
        });

    if (!found) {
        cout << "[Error] Actor ID " << actorId << " not found.\n";
    }
}

void MovieApp::setMovieRating(int movieId, int rating) {
    // Ensure the rating is between 1 and 10
    if (rating < 1 || rating > 10) {
        cout << "[Error] Invalid rating value. Must be between 1 and 10.\n";
        return;
    }

    bool found = false;
    movieList.display([&](Movie& m) {
        if (m.getId() == movieId) {
            m.setRating(rating);      // Update the movie's rating
            movieRatings.insert(m);  // Insert the movie into the BST
            cout << "[Success] Movie (ID=" << movieId << ") rating updated to " << rating << "\n";
            found = true;
            return true; // Stop traversal
        }
        return false;
        });

    if (!found) {
        cout << "[Error] Movie ID " << movieId << " not found.\n";
    }
}

void MovieApp::recommendMoviesByRating(int minRating, int maxRating) const {
    cout << "Movies with ratings between " << minRating << " and " << maxRating << ":\n";
    movieRatings.displayMoviesInRange(minRating, maxRating);
}

void MovieApp::recommendActorsByRating(int minRating, int maxRating) const {
    cout << "Actors with ratings between " << minRating << " and " << maxRating << ":\n";
    actorRatings.displayActorsInRange(minRating, maxRating);
}


