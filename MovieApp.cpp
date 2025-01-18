#include "MovieApp.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>   // for atoi, etc.
#include <cstring>   // for strcmp
#include "ActorGraph.h"
using namespace std;

// A small helper to trim quotes/spaces
static std::string trimQuotes(const std::string& str) {
    size_t start = str.find_first_not_of(" \"");
    size_t end = str.find_last_not_of(" \"");
    if (start == std::string::npos || end == std::string::npos) {
        return "";
    }
    return str.substr(start, end - start + 1);
}

MovieApp::MovieApp()
    : nextActorId(1000), // or pick any starting ID
    nextMovieId(5000)
{}

//------------------------------------------------------------------------------

void MovieApp::readActors(const std::string& filename) {
    ifstream fin(filename.c_str());
    if (!fin.is_open()) {
        cerr << "Error: Could not open file \"" << filename << "\"\n";
        return;
    }

    // skip header
    string header;
    getline(fin, header);

    string line;
    while (getline(fin, line)) {
        if (line.empty()) continue;

        // parse
        // "id,name,birth"
        stringstream ss(line);
        string idStr, nameStr, birthStr;
        getline(ss, idStr, ',');
        getline(ss, nameStr, ',');
        getline(ss, birthStr, ',');

        int id = atoi(idStr.c_str());
        int birth = atoi(birthStr.c_str());
        nameStr = trimQuotes(nameStr);

        Actor actor(nameStr.c_str(), birth, id);
        actorList.add(actor);

        if (id >= nextActorId) {
            nextActorId = id + 1;
        }
    }
    fin.close();
}

void MovieApp::readMovies(const std::string& filename) {
    ifstream fin(filename.c_str());
    if (!fin.is_open()) {
        cerr << "Error: Could not open file \"" << filename << "\"\n";
        return;
    }

    // skip header
    string header;
    getline(fin, header);

    string line;
    while (getline(fin, line)) {
        if (line.empty()) continue;

        // parse
        // "id,title,year"
        stringstream ss(line);
        string idStr, titleStr, yearStr;
        getline(ss, idStr, ',');
        getline(ss, titleStr, ',');
        getline(ss, yearStr, ',');

        int id = atoi(idStr.c_str());
        int year = atoi(yearStr.c_str());
        titleStr = trimQuotes(titleStr);

        Movie movie(titleStr.c_str(), "", year, id);
        movieList.add(movie);

        if (id >= nextMovieId) {
            nextMovieId = id + 1;
        }
    }
    fin.close();
}

void MovieApp::readCast(const std::string& filename) {
    ifstream fin(filename.c_str());
    if (!fin.is_open()) {
        cerr << "Error: Could not open file \"" << filename << "\"\n";
        return;
    }
    // skip header
    string header;
    getline(fin, header);

    string line;
    while (getline(fin, line)) {
        if (line.empty()) continue;
        // parse "person_id,movie_id"
        stringstream ss(line);
        string pidStr, midStr;
        getline(ss, pidStr, ',');
        getline(ss, midStr, ',');

        if (pidStr.empty() || midStr.empty()) continue;

        int personId = atoi(pidStr.c_str());
        int movieId = atoi(midStr.c_str());

        // find the Actor and Movie
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
// 2) "Add New" items
//------------------------------------------------------------------------------

void MovieApp::addNewActor(const std::string& name, int birthYear) {
    Actor actor(name.c_str(), birthYear, nextActorId);
    ++nextActorId;
    actorList.add(actor);
    cout << "Added new actor: \"" << name << "\" (ID=" << actor.getId() << ")\n";
}

void MovieApp::addNewMovie(const std::string& title, const std::string& plot, int releaseYear) {
    Movie movie(title.c_str(), plot.c_str(), releaseYear, nextMovieId);
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
        if (strcmp(m.getTitle(), movieTitle.c_str()) == 0) {
            targetMovie = &m;
            return true;
        }
        return false;
        });

    if (!targetMovie) {
        cout << "[Error] Movie \"" << movieTitle << "\" not found.\n";
        return;
    }

    Actor* targetActor = nullptr;
    actorList.display([&](Actor& a) {
        if (strcmp(a.getName(), actorName.c_str()) == 0) {
            targetActor = &a;
            return true;
        }
        return false;
        });

    if (!targetActor) {
        cout << "[Error] Actor \"" << actorName << "\" not found.\n";
        return;
    }

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
            a.setName(newName.c_str());
            a.setBirthYear(newYearOfBirth);
            cout << "Updated Actor ID " << actorId
                << " => Name: " << newName
                << ", BirthYear: " << newYearOfBirth << endl;
            found = true;
            return true;
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
            m.setTitle(newTitle.c_str());
            m.setPlot(newPlot.c_str());
            m.setReleaseYear(newReleaseYear);
            cout << "Updated Movie ID " << movieId
                << " => Title: " << newTitle
                << ", Plot: " << newPlot
                << ", Year: " << newReleaseYear << endl;
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

void MovieApp::displayAllActors() const {
    if (actorList.isEmpty()) {
        cout << "No actors found.\n";
        return;
    }

    static const int MAX_ACTORS = 2000;
    Actor actorArray[MAX_ACTORS];
    int count = 0;

    // Collect all actors into an array
    actorList.display([&](const Actor& actor) {
        if (count < MAX_ACTORS) {
            actorArray[count++] = actor;
        }
        return false; // continue traversal
        });

    // Sort actors alphabetically by name
    for (int i = 0; i < count - 1; ++i) {
        for (int j = 0; j < count - i - 1; ++j) {
            if (strcmp(actorArray[j].getName(), actorArray[j + 1].getName()) > 0) {
                std::swap(actorArray[j], actorArray[j + 1]);
            }
        }
    }

    // Display sorted actors
    cout << "All Actors (alphabetical order):\n";
    for (int i = 0; i < count; ++i) {
        actorArray[i].displayDetails();
    }
}


void MovieApp::displayAllMovies() const {
    if (movieList.isEmpty()) {
        std::cout << "No movies found.\n";
        return;
    }

    static const int MAX_MOVIES = 2000;
    // Allocate on the heap
    Movie* movieArray = new Movie[MAX_MOVIES];
    int count = 0;

    // Collect all movies into the array
    movieList.display([&](const Movie& movie) {
        if (count < MAX_MOVIES) {
            movieArray[count++] = movie;
        }
        return false; // Continue traversal
        });

    if (count == 0) {
        std::cout << "No movies found.\n";
        delete[] movieArray; // Clean up
        return;
    }

    // Bubble Sort to sort movies by title
    for (int i = 0; i < count - 1; ++i) {
        bool swapped = false; // Optimization to stop if no swaps occurred
        for (int j = 0; j < count - i - 1; ++j) {
            if (std::strcmp(movieArray[j].getTitle(), movieArray[j + 1].getTitle()) > 0) {
                std::swap(movieArray[j], movieArray[j + 1]);
                swapped = true;
            }
        }
        if (!swapped) {
            break; // Array is already sorted
        }
    }

    // Display sorted movies
    std::cout << "All Movies (alphabetical order):\n";
    for (int i = 0; i < count; ++i) {
        movieArray[i].displayDetails();
    }

    // Clean up heap memory
    delete[] movieArray;
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

    // 1. Test Adding New Actors
    cout << "\n-- Test: Adding New Actors --\n";
    addNewActor("Leonardo DiCaprio", 1974);
    addNewActor("Kate Winslet", 1975);
    addNewActor("Brad Pitt", 1963);
    displayAllActors();

    // 2. Test Adding New Movies
    cout << "\n-- Test: Adding New Movies --\n";
    addNewMovie("Inception", "A thief who steals corporate secrets through dream-sharing technology.", 2010);
    addNewMovie("Titanic", "A love story that unfolds aboard the ill-fated RMS Titanic.", 1997);
    addNewMovie("Fight Club", "An insomniac office worker and a soap maker form an underground fight club.", 1999);
    displayAllMovies();

    // 3. Test Adding Actors to Movies
    cout << "\n-- Test: Adding Actors to Movies --\n";
    addActorToMovie("Leonardo DiCaprio", "Inception");
    addActorToMovie("Kate Winslet", "Titanic");
    addActorToMovie("Brad Pitt", "Fight Club");
    addActorToMovie("Leonardo DiCaprio", "Titanic"); // Leonardo in Titanic as well
    displayActorsInMovie("Inception");
    displayActorsInMovie("Titanic");
    displayActorsInMovie("Fight Club");

    // 4. Test Updating Actor Details
    cout << "\n-- Test: Updating Actor Details --\n";
    // Assume actor IDs are assigned starting from 1 as per addNewActor implementation
    updateActorDetails(1, "Leonardo Wilhelm DiCaprio", 1974);
    updateActorDetails(2, "Katherine Winslet", 1975);
    displayAllActors();

    // 5. Test Updating Movie Details
    cout << "\n-- Test: Updating Movie Details --\n";
    // Assume movie IDs are assigned starting from 1 as per addNewMovie implementation
    updateMovieDetails(1, "Inception (Edited)", "A skilled thief is offered a chance to erase his past.", 2010);
    displayAllMovies();

    // 6. Test Displaying Movies of an Actor
    cout << "\n-- Test: Displaying Movies of an Actor --\n";
    displayMoviesOfActor("Leonardo Wilhelm DiCaprio");
    displayMoviesOfActor("Katherine Winslet");
    displayMoviesOfActor("Brad Pitt");

    // 7. Test Displaying Actors by Age Range
    cout << "\n-- Test: Displaying Actors by Age Range (40-60) --\n";
    displayActorsByAge(40, 60);

    // 8. Test Displaying Recent Movies (Released in Last 3 Years)
    cout << "\n-- Test: Displaying Recent Movies (Released in Last 3 Years) --\n";
    displayRecentMovies();

    // 9. Test Displaying Actors Known by an Actor
    cout << "\n-- Test: Displaying Actors Known by an Actor --\n";
    // For simplicity, assuming 'known by' is predefined or not implemented
    // You may need to implement relationships for this feature
    // Here, we'll just display based on current data
    displayActorsKnownBy("Leonardo Wilhelm DiCaprio");
    displayActorsKnownBy("Katherine Winslet");
    displayActorsKnownBy("Brad Pitt");

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