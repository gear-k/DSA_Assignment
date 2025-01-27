#include "ActorGraph.h"
#include <cstring> // for memset, etc.

// ====================== BFSQueue Implementation ========================

ActorGraph::BFSQueue::BFSQueue()
    : front(0), rear(-1), count(0)
{
    // Initialize all Pair elements in data to zero
    for (int i = 0; i < 2000; ++i) {
        data[i].idx = 0;
        data[i].depth = 0;
    }
}

bool ActorGraph::BFSQueue::isEmpty() const {
    return (count == 0);
}

bool ActorGraph::BFSQueue::isFull() const {
    return (count == 2000);
}

bool ActorGraph::BFSQueue::enqueue(int i, int d) {
    if (isFull()) return false;
    rear = (rear + 1) % 2000;
    data[rear].idx = i;
    data[rear].depth = d;
    count++;
    return true;
}

bool ActorGraph::BFSQueue::dequeue(Pair& out) {
    if (isEmpty()) return false;
    out = data[front];
    front = (front + 1) % 2000;
    count--;
    return true;
}


// ====================== Graph Building (HashTable-based) ========================

/**
 * buildActorGraph
 *   - Iterates over all actors in `actorTable`, storing their IDs in actorIds[].
 *   - Iterates over all movies in `movieTable`, collects each movie's cast,
 *     then connects every pair of those actors in the adjacency list.
 */
void ActorGraph::buildActorGraph(
    const HashTable<Actor>& actorTable,
    const HashTable<Movie>& movieTable,
    int actorIds[],
    int& actorCount,
    List<int>* adjacencyLists
) {
    // 1) Collect all actor IDs into actorIds[]
    actorCount = 0;
    actorTable.forEach([&](const Actor& actor) -> bool {
        if (actorCount < MAX_ACTORS) {
            actorIds[actorCount++] = actor.getId();
        }
        return false; // keep going
        });

    // 2) For each movie, gather the indices of all its actors, then link them
    movieTable.forEach([&](const Movie& mov) -> bool {
        // We collect up to 300 actor indices from a single movie
        static const int TEMP_SIZE = 300;
        int* tmpIdx = new int[TEMP_SIZE];
        int tmpCount = 0;

        // We still rely on Movie::getActors() returning a List<Actor>.
        // Then we find each actor's index in actorIds[].
        mov.getActors().display([&](const Actor& a) {
            int idx = findActorIndexInArray(a.getId(), actorIds, actorCount);
            if (idx != -1 && tmpCount < TEMP_SIZE) {
                tmpIdx[tmpCount++] = idx;
            }
            return false;
            });

        // 3) For each pair in tmpIdx, add them to each other's adjacency list
        for (int i = 0; i < tmpCount; i++) {
            for (int j = i + 1; j < tmpCount; j++) {
                adjacencyLists[tmpIdx[i]].add(tmpIdx[j]);
                adjacencyLists[tmpIdx[j]].add(tmpIdx[i]);
            }
        }

        delete[] tmpIdx;
        return false; // continue iteration
        });
}

/**
 * findActorIndexInArray
 *   - returns the index in actorIds[] of the given actorId, or -1 if not found
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
 * findConnectedActors
 *   - BFS from a starting index, up to `maxDepth` edges away.
 *   - Returns a List<int> of discovered actor indices (not IDs).
 */
List<int> ActorGraph::findConnectedActors(
    int startIndex,
    const List<int>* adjacencyLists,
    int maxDepth
) {
    List<int> discoveredIndices;

    // Create a visited array to track visited indices
    bool* visited = new bool[MAX_ACTORS];
    for (int i = 0; i < MAX_ACTORS; ++i) {
        visited[i] = false;
    }

    BFSQueue queue;
    queue.enqueue(startIndex, 0);
    visited[startIndex] = true;

    while (!queue.isEmpty()) {
        BFSQueue::Pair current;
        if (!queue.dequeue(current)) break;

        int curIdx = current.idx;
        int curDepth = current.depth;

        // For neighbors
        if (curDepth < maxDepth) {
            adjacencyLists[curIdx].display([&](int neighborIdx) {
                if (!visited[neighborIdx]) {
                    visited[neighborIdx] = true;
                    discoveredIndices.add(neighborIdx);
                    queue.enqueue(neighborIdx, curDepth + 1);
                }
                return false; // keep going
                });
        }
    }

    delete[] visited;
    return discoveredIndices;
}
