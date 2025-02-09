#include "ActorGraph.h"
#include <cstring>
#include <iostream>
#include <cassert>

/**
 * @brief Helper function to append an actor ID to a dynamic array.
 *
 * If the array's capacity is reached, its capacity is doubled and the existing
 * elements are copied to a new array.
 *
 * @param arr       Reference to the pointer of the actor IDs array.
 * @param count     Reference to the current count of elements in the array.
 * @param capacity  Reference to the current capacity of the array.
 * @param value     The actor ID value to append.
 */
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
    assert(count < capacity);  // Verify that there is room.
    arr[count++] = value;
}

/**
 * @brief Constructs an empty BFSQueue.
 *
 * Initializes the BFS queue with default values: front = 0, rear = -1, and count = 0.
 */
ActorGraph::BFSQueue::BFSQueue()
    : front(0), rear(-1), count(0) {
}

/**
 * @brief Checks whether the BFS queue is empty.
 *
 * @return true if the queue has no elements; false otherwise.
 */
bool ActorGraph::BFSQueue::isEmpty() const {
    return (count == 0);
}

/**
 * @brief Checks whether the BFS queue is full.
 *
 * The queue is considered full when it reaches a fixed size of 2000 elements.
 *
 * @return true if the queue is full; false otherwise.
 */
bool ActorGraph::BFSQueue::isFull() const {
    return (count == 2000);
}

/**
 * @brief Enqueues an element into the BFS queue.
 *
 * Adds an element containing an actor index and its BFS depth. If the queue is full,
 * the function returns false.
 *
 * @param i The actor index to enqueue.
 * @param d The depth associated with the actor.
 * @return true if the element was successfully enqueued; false otherwise.
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
 * @brief Dequeues an element from the BFS queue.
 *
 * Removes the element at the front of the queue and assigns it to the output parameter.
 *
 * @param out Reference to a Pair where the dequeued element will be stored.
 * @return true if an element was successfully dequeued; false if the queue was empty.
 */
bool ActorGraph::BFSQueue::dequeue(Pair& out) {
    if (isEmpty()) return false;
    out = data[front];
    front = (front + 1) % 2000;
    count--;
    return true;
}

/**
 * @brief Builds the actor graph using movie cast information.
 *
 * This function creates a dynamic array of actor IDs and constructs an adjacency list
 * representing the actor graph. Two actors are connected if they have appeared together
 * in a movie.
 *
 * @param actorTable      A hash table containing Actor objects.
 * @param movieTable      A hash table containing Movie objects.
 * @param actorIds        Reference to a pointer that will point to the array of actor IDs.
 * @param actorCount      Reference to an integer that will hold the total number of actors.
 * @param adjacencyLists  Reference to a pointer that will point to the array of adjacency lists.
 */
void ActorGraph::buildActorGraph(
    const HashTable<Actor>& actorTable,
    const HashTable<Movie>& movieTable,
    int*& actorIds,
    int& actorCount,
    List<int>*& adjacencyLists
) {
    int capacity = INITIAL_ACTOR_CAPACITY;
    actorIds = new int[capacity];
    actorCount = 0;

    // Iterate through each actor in the actor table and store valid actor IDs.
    actorTable.forEach([&](const Actor& actor) -> bool {
        if (actor.getId() > 0) {
            appendActorId(actorIds, actorCount, capacity, actor.getId());
        }
        return false;
        });

    // Initialize adjacency lists for all actors.
    adjacencyLists = new List<int>[actorCount];

    // Build graph edges based on movie cast connections.
    movieTable.forEach([&](const Movie& mov) -> bool {
        static const int TEMP_SIZE = 300;
        int* tmpIdx = new int[TEMP_SIZE];
        int tmpCount = 0;

        // For each actor in the movie, find its index in the actorIds array.
        mov.getActors().forEach([&](const Actor& a) {
            int idx = findActorIndexInArray(a.getId(), actorIds, actorCount);
            if (idx != -1 && tmpCount < TEMP_SIZE) {
                tmpIdx[tmpCount++] = idx;
            }
            return false;
            });

        // Create bidirectional connections between actors who starred together.
        for (int i = 0; i < tmpCount; i++) {
            for (int j = i + 1; j < tmpCount; j++) {
                adjacencyLists[tmpIdx[i]].add(tmpIdx[j]);
                adjacencyLists[tmpIdx[j]].add(tmpIdx[i]);
            }
        }

        delete[] tmpIdx;
        return false;
        });
}

/**
 * @brief Finds the index of an actor in the actorIds array.
 *
 * Searches through the actorIds array for a given actor ID.
 *
 * @param actorId  The actor ID to search for.
 * @param actorIds The array of actor IDs.
 * @param count    The number of elements in the actorIds array.
 * @return The index of the actor if found; -1 otherwise.
 */
int ActorGraph::findActorIndexInArray(int actorId, const int actorIds[], int count) {
    for (int i = 0; i < count; i++) {
        if (actorIds[i] == actorId) {
            return i;
        }
    }
    return -1;
}

/**
 * @brief Finds all actors connected to the start actor within a specified depth using BFS.
 *
 * Performs a breadth-first search (BFS) on the actor graph starting from a given actor index.
 * It returns a list of actor indices that are reachable within the given maximum depth.
 *
 * @param startIndex     The starting index for the BFS.
 * @param adjacencyLists The array of adjacency lists representing the actor graph.
 * @param actorCount     The total number of actors in the graph.
 * @param maxDepth       The maximum depth to search.
 * @return A list of actor indices that are connected to the starting actor.
 */
List<int> ActorGraph::findConnectedActors(
    int startIndex,
    const List<int>* adjacencyLists,
    int actorCount,
    int maxDepth
) {
    List<int> discoveredIndices;
    bool* visited = new bool[actorCount] {};  // All elements are initialized to false.
    BFSQueue queue;

    // Start the BFS from the startIndex.
    queue.enqueue(startIndex, 0);
    visited[startIndex] = true;

    while (!queue.isEmpty()) {
        BFSQueue::Pair current;
        if (!queue.dequeue(current))
            break;

        int curIdx = current.idx;
        int curDepth = current.depth;

        // If the current depth is less than the maximum, explore adjacent nodes.
        if (curDepth < maxDepth) {
            adjacencyLists[curIdx].forEach([&](int neighborIdx) {
                if (!visited[neighborIdx]) {
                    visited[neighborIdx] = true;
                    discoveredIndices.add(neighborIdx);
                    queue.enqueue(neighborIdx, curDepth + 1);
                }
                return false;
                });
        }
    }

    delete[] visited;
    return discoveredIndices;
}
