#ifndef ACTOR_GRAPH_H
#define ACTOR_GRAPH_H

#include "HashTable.h"
#include "Actor.h"
#include "Movie.h"
#include "List.h"

/**
 * ActorGraph class for building adjacency (who has worked with whom) from
 * an actorTable and movieTable, then performing BFS up to a specified depth.
 */
class ActorGraph {
public:
    static const int MAX_ACTORS = 2000;

    // A minimal queue for BFS
    struct BFSQueue {
        struct Pair {
            int idx;
            int depth;
        };
        Pair data[2000];
        int front;
        int rear;
        int count;

        BFSQueue();
        bool isEmpty() const;
        bool isFull() const;
        bool enqueue(int i, int d);
        bool dequeue(Pair& out);
    };

    // Main graph building function (HashTable-based):
    static void buildActorGraph(
        const HashTable<Actor>& actorTable,
        const HashTable<Movie>& movieTable,
        int actorIds[],
        int& actorCount,
        List<int>* adjacencyLists
    );

    // Utility to find an actor's index in actorIds[] array
    static int findActorIndexInArray(int actorId, const int actorIds[], int count);

    // BFS that returns a list of discovered actor indices up to `maxDepth`
    static List<int> findConnectedActors(
        int startIndex,
        const List<int>* adjacencyLists,
        int maxDepth = 2
    );
};

#endif // ACTOR_GRAPH_H
