#include "ActorGraph.h"
#include <cstring>

ActorGraph::BFSQueue::BFSQueue() : front(0), rear(-1), count(0) {}

bool ActorGraph::BFSQueue::isEmpty() const {
    return count == 0;
}

bool ActorGraph::BFSQueue::enqueue(int i, int d) {
    if (isFull()) return false;
    rear = (rear + 1) % 2000;
    data[rear].idx = i;
    data[rear].depth = d;
    count++;
    return true;
}

bool ActorGraph::BFSQueue::isFull() const {
    return count == 2000;
}


bool ActorGraph::BFSQueue::dequeue(Pair& out) {
    if (isEmpty()) return false;
    out = data[front];
    front = (front + 1) % 2000;
    count--;
    return true;
}

void ActorGraph::buildActorGraph(
    const List<Actor>& actorList,
    const List<Movie>& movieList,
    int actorIds[],
    int& actorCount,
    List<int>* adjacencyLists
) {
    actorCount = 0;

    // 1) Collect all actor IDs
    actorList.display([&](const Actor& actor) {
        if (actorCount < MAX_ACTORS) {
            actorIds[actorCount] = actor.getId();
            actorCount++;
        }
        return false;
        });

    // 2) Build adjacency lists
    movieList.display([&](const Movie& mov) {
        int tmpIdx[300];
        int tmpCount = 0;

        mov.getActors().display([&](const Actor& a) {
            int aIndex = findActorIndexInArray(a.getId(), actorIds, actorCount);
            if (aIndex != -1 && tmpCount < 300) {
                tmpIdx[tmpCount++] = aIndex;
            }
            return false;
            });

        // Link actors pairwise
        for (int i = 0; i < tmpCount; i++) {
            for (int j = i + 1; j < tmpCount; j++) {
                adjacencyLists[tmpIdx[i]].add(tmpIdx[j]);
                adjacencyLists[tmpIdx[j]].add(tmpIdx[i]);
            }
        }
        return false;
        });
}

int ActorGraph::findActorIndexInArray(int actorId, const int actorIds[], int count) {
    for (int i = 0; i < count; i++) {
        if (actorIds[i] == actorId) {
            return i;
        }
    }
    return -1;
}

List<int> ActorGraph::findConnectedActors(
    int startIndex,
    const List<int>* adjacencyLists,
    int maxDepth
) {
    List<int> discoveredIndices;
    bool visited[MAX_ACTORS] = { false };

    BFSQueue q;
    q.enqueue(startIndex, 0);
    visited[startIndex] = true;

    BFSQueue::Pair current;
    while (!q.isEmpty()) {
        if (!q.dequeue(current)) break;
        int curIdx = current.idx;
        int curDepth = current.depth;

        if (curDepth < maxDepth) {
            adjacencyLists[curIdx].display([&](int neighborIdx) {
                if (!visited[neighborIdx]) {
                    visited[neighborIdx] = true;
                    discoveredIndices.add(neighborIdx);
                    q.enqueue(neighborIdx, curDepth + 1);
                }
                return false;
                });
        }
    }

    return discoveredIndices;
}