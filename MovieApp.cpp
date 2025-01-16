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
    actorList.display([](const Actor& actor) {
        actor.displayDetails();
        return false; // continue
        });
}

void MovieApp::displayAllMovies() const {
    if (movieList.isEmpty()) {
        cout << "No movies found.\n";
        return;
    }
    movieList.display([](const Movie& movie) {
        movie.displayDetails();
        return false; // continue
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
        if (m.hasActor(actorName.c_str())) {
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
        if (strcmp(m.getTitle(), movieTitle.c_str()) == 0) {
            foundMovie = true;
            cout << "Actors in \"" << movieTitle << "\":\n";
            m.getActors().display([](const Actor& a) {
                cout << " - " << a.getName() << endl;
                return false;
                });
            return true; // break
        }
        return false;
        });
    if (!foundMovie) {
        cout << "Movie \"" << movieTitle << "\" not found.\n";
    }
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

    // 1) Collect all actor IDs in an array (actorIds[]).
    //    We'll use the order in which they appear in actorList.
    actorList.display([&](const Actor& actor) {
        if (actorCount < MAX_ACTORS) {
            actorIds[actorCount] = actor.getId();
            actorCount++;
        }
        return false;
        });

    // 2) For each actor, adjacencyLists[i] starts empty. We'll fill them now.
    //    We do so by scanning each movie, collecting the actor indices, then linking them.
    //    a) For each movie, gather its actors into a small array of indices
    movieList.display([&](const Movie& mov) {
        // gather actor indices
        int tmpIdx[300];
        int tmpCount = 0;

        mov.getActors().display([&](const Actor& a) {
            // find the index of a.getId() in actorIds[]
            int aIndex = findActorIndexInArray(a.getId(), actorIds, actorCount);
            if (aIndex != -1 && tmpCount < 300) {
                tmpIdx[tmpCount++] = aIndex;
            }
            return false;
            });

        // link them pairwise
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

    BFSQueue() : front(0), rear(-1), count(0) {}

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