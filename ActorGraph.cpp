/***************************************************************************
 * ActorGraph.cpp
 *
 * Team:            Gearoid, Cedric
 * Group:           1
 * Student IDs:     S10241866, S10241549
 *
 * Features Highlight:
 *   - Implements a BFS-based queue for graph traversal.
 *   - Builds an actor graph using hash tables for actors and movies.
 *   - Finds connected actors using breadth-first search up to a given depth.
 *
 ***************************************************************************/

#include "ActorGraph.h"
#include <cstring>   // for memset, if needed
#include <iostream>  // For logging output

 // ====================== Helper for Dynamic Array Growth ========================

 // Appends a value to a dynamic array of actor IDs.
 // If the array is full (count >= capacity), it doubles the capacity.
static void appendActorId(int*& arr, int& count, int& capacity, int value) {
    if (count >= capacity) {
        int newCapacity = capacity * 2;
        int* newArr = new int[newCapacity];
        for (int i = 0; i < count; ++i) {
            newArr[i] = arr[i];
        }
        delete[] arr;
        arr = newArr;
        capacity = newCapacity;
    }
    arr[count++] = value;
}

// ====================== BFSQueue Implementation ========================

/**
 * @brief Constructs a new BFSQueue object.
 *
 * Initializes the front index to 0, rear index to -1, and count to 0.
 * Also initializes each element of the queue data array.
 */
ActorGraph::BFSQueue::BFSQueue()
    : front(0), rear(-1), count(0)
{
    // Initialize all Pair elements in data to zero
    for (int i = 0; i < 2000; ++i) {
        data[i].idx = 0;
        data[i].depth = 0;
    }
}

/**
 * @brief Checks if the BFSQueue is empty.
 *
 * @return true if the queue is empty; false otherwise.
 */
bool ActorGraph::BFSQueue::isEmpty() const {
    return (count == 0);
}

/**
 * @brief Checks if the BFSQueue is full.
 *
 * @return true if the queue is full; false otherwise.
 */
bool ActorGraph::BFSQueue::isFull() const {
    return (count == 2000);
}

/**
 * @brief Enqueues an element into the BFSQueue.
 *
 * Inserts the given actor index and its associated depth into the queue.
 *
 * @param i The actor index to enqueue.
 * @param d The depth (level) associated with the actor.
 * @return true if enqueue is successful; false if the queue is full.
 */
bool ActorGraph::BFSQueue::enqueue(int i, int d) {
    if (isFull()) return false;
    rear = (rear + 1) % 2000;
    data[rear].idx = i;
    data[rear].depth = d;
    count++;
    return true;
}

/**
 * @brief Dequeues an element from the BFSQueue.
 *
 * Removes the front element from the queue and returns it via the out parameter.
 *
 * @param out Reference to a Pair that will store the dequeued element.
 * @return true if dequeue is successful; false if the queue is empty.
 */
bool ActorGraph::BFSQueue::dequeue(Pair& out) {
    if (isEmpty()) return false;
    out = data[front];
    front = (front + 1) % 2000;
    count--;
    return true;
}

// ====================== Graph Building (Dynamic, HashTable-based) ========================

/**
 * @brief Builds the actor graph based on movie cast information.
 *
 * This function collects all actor IDs from the actorTable dynamically
 * (growing the array as needed) and then, for each movie in the movieTable, it
 * identifies the indices of the actors in that movie. It then adds each pair
 * of actors (from the same movie) to each other's adjacency list.
 *
 * @param actorTable The hash table containing all actors.
 * @param movieTable The hash table containing all movies.
 * @param actorIds Reference to a pointer that will point to a dynamically allocated array of actor IDs.
 * @param actorCount Reference to an integer to hold the total number of actors.
 * @param adjacencyLists Reference to a pointer that will point to a dynamically allocated array of List<int>,
 *                       where each element represents an actor's adjacency list.
 */
void ActorGraph::buildActorGraph(
    const HashTable<Actor>& actorTable,
    const HashTable<Movie>& movieTable,
    int*& actorIds,
    int& actorCount,
    List<int>*& adjacencyLists
) {
    // 1) Dynamically allocate the actorIds array with an initial capacity.
    int capacity = INITIAL_ACTOR_CAPACITY; // Defined in ActorGraph.h (e.g., 2000)
    actorIds = new int[capacity];
    actorCount = 0;
    actorTable.forEach([&](const Actor& actor) -> bool {
        if (actor.getId() > 0) {  // Only add valid actors.
            appendActorId(actorIds, actorCount, capacity, actor.getId());
        }
        return false; // continue iterating
        });
    std::cout << "[DEBUG] buildActorGraph: Collected " << actorCount << " actors." << std::endl;

    // 2) Dynamically allocate the adjacency lists array (one list per actor).
    adjacencyLists = new List<int>[actorCount];

    // 3) For each movie, gather the indices of all its actors, then link them.
    movieTable.forEach([&](const Movie& mov) -> bool {
        std::cout << "[DEBUG] Processing movie ID " << mov.getId()
            << " (" << mov.getTitle() << ")." << std::endl;
        // Allocate a temporary array for actor indices from this movie.
        static const int TEMP_SIZE = 300;
        int* tmpIdx = new int[TEMP_SIZE];
        int tmpCount = 0;

        // Retrieve the cast from the movie (assume mov.getActors() returns a List<Actor>).
        mov.getActors().display([&](const Actor& a) {
            int idx = findActorIndexInArray(a.getId(), actorIds, actorCount);
            if (idx == -1) {
                std::cout << "[DEBUG] Could not find actor ID " << a.getId()
                    << " (" << a.getName() << ") in actorIds array." << std::endl;
            }
            else {
                if (tmpCount < TEMP_SIZE) {
                    tmpIdx[tmpCount++] = idx;
                }
            }
            return false; // continue iterating
            });

        std::cout << "[DEBUG] Movie ID " << mov.getId() << " (" << mov.getTitle()
            << ") has " << tmpCount << " actors." << std::endl;

        // For each pair in tmpIdx, add them to each other's adjacency list.
        for (int i = 0; i < tmpCount; i++) {
            for (int j = i + 1; j < tmpCount; j++) {
                adjacencyLists[tmpIdx[i]].add(tmpIdx[j]);
                adjacencyLists[tmpIdx[j]].add(tmpIdx[i]);
            }
        }

        delete[] tmpIdx;
        return false; // continue iterating over movies
        });
}

/**
 * @brief Finds the index of an actor in the actorIds array.
 *
 * Searches through the actorIds array to locate the index corresponding to the given actorId.
 *
 * @param actorId The actor ID to search for.
 * @param actorIds The array containing actor IDs.
 * @param count The number of elements in actorIds.
 * @return The index of the actor if found; -1 otherwise.
 */
int ActorGraph::findActorIndexInArray(int actorId, const int actorIds[], int count) {
    for (int i = 0; i < count; i++) {
        if (actorIds[i] == actorId) {
            return i;
        }
    }
    std::cout << "[DEBUG] findActorIndexInArray: Actor ID " << actorId
        << " not found in array (total count: " << count << ")." << std::endl;
    return -1;
}

/**
 * @brief Finds all actors connected to the start actor within a given maximum depth.
 *
 * Uses a breadth-first search (BFS) to traverse the graph defined by adjacencyLists,
 * starting from the specified index. The search stops when the maximum depth is reached.
 *
 * @param startIndex The index of the starting actor.
 * @param adjacencyLists Array of adjacency lists for all actors.
 * @param actorCount The total number of actors (size of the dynamic arrays).
 * @param maxDepth The maximum depth to search.
 * @return A List<int> containing the indices of all connected actors discovered.
 */
List<int> ActorGraph::findConnectedActors(
    int startIndex,
    const List<int>* adjacencyLists,
    int actorCount,  // new parameter to know the actual number of actors
    int maxDepth
) {
    List<int> discoveredIndices;

    // Create a visited array to track which actor indices have been visited.
    bool* visited = new bool[actorCount];
    for (int i = 0; i < actorCount; ++i) {
        visited[i] = false;
    }

    BFSQueue queue;

    queue.enqueue(startIndex, 0);
    visited[startIndex] = true;

    while (!queue.isEmpty()) {
        BFSQueue::Pair current;
        if (!queue.dequeue(current))
            break;

        int curIdx = current.idx;
        int curDepth = current.depth;


        // For each neighbor of the current index, if we haven't reached maxDepth, enqueue it.
        if (curDepth < maxDepth) {
            adjacencyLists[curIdx].display([&](int neighborIdx) {
                if (!visited[neighborIdx]) {
                    visited[neighborIdx] = true;
                    discoveredIndices.add(neighborIdx);

                    queue.enqueue(neighborIdx, curDepth + 1);
                }
                return false; // continue iterating
                });
        }
    }

    delete[] visited;
    return discoveredIndices;
}
