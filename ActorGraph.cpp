#include "ActorGraph.h"
#include <cstring>
#include <iostream>

// Helper function to append a value to a dynamic array of actor IDs.
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

// Constructor initializes an empty BFS queue with default values.
ActorGraph::BFSQueue::BFSQueue()
    : front(0), rear(-1), count(0) {
}

// Checks if the BFS queue is empty.
bool ActorGraph::BFSQueue::isEmpty() const {
    return (count == 0);
}

// Checks if the BFS queue is full.
bool ActorGraph::BFSQueue::isFull() const {
    return (count == 2000);
}

// Adds an element (actor index and depth) to the BFS queue.
bool ActorGraph::BFSQueue::enqueue(int i, int d) {
    if (isFull()) return false;
    rear = (rear + 1) % 2000;
    data[rear].idx = i;
    data[rear].depth = d;
    count++;
    return true;
}

// Removes an element from the BFS queue and returns it via the output parameter.
bool ActorGraph::BFSQueue::dequeue(Pair& out) {
    if (isEmpty()) return false;
    out = data[front];
    front = (front + 1) % 2000;
    count--;
    return true;
}

// Builds the actor graph based on movie cast information.
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

    actorTable.forEach([&](const Actor& actor) -> bool {
        if (actor.getId() > 0) {
            appendActorId(actorIds, actorCount, capacity, actor.getId());
        }
        return false;
        });

    adjacencyLists = new List<int>[actorCount];

    movieTable.forEach([&](const Movie& mov) -> bool {
        static const int TEMP_SIZE = 300;
        int* tmpIdx = new int[TEMP_SIZE];
        int tmpCount = 0;

        mov.getActors().display([&](const Actor& a) {
            int idx = findActorIndexInArray(a.getId(), actorIds, actorCount);
            if (idx != -1 && tmpCount < TEMP_SIZE) {
                tmpIdx[tmpCount++] = idx;
            }
            return false;
            });

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

// Finds the index of an actor in the actorIds array.
int ActorGraph::findActorIndexInArray(int actorId, const int actorIds[], int count) {
    for (int i = 0; i < count; i++) {
        if (actorIds[i] == actorId) {
            return i;
        }
    }
    return -1;
}

// Finds all actors connected to the start actor within a given maximum depth using BFS.
List<int> ActorGraph::findConnectedActors(
    int startIndex,
    const List<int>* adjacencyLists,
    int actorCount,
    int maxDepth
) {
    List<int> discoveredIndices;
    bool* visited = new bool[actorCount] {};
    BFSQueue queue;

    queue.enqueue(startIndex, 0);
    visited[startIndex] = true;

    while (!queue.isEmpty()) {
        BFSQueue::Pair current;
        if (!queue.dequeue(current))
            break;

        int curIdx = current.idx;
        int curDepth = current.depth;

        if (curDepth < maxDepth) {
            adjacencyLists[curIdx].display([&](int neighborIdx) {
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
