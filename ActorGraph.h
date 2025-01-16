#ifndef ACTOR_GRAPH_H
#define ACTOR_GRAPH_H

#include "List.h" 
#include "Actor.h"
#include "Movie.h"

class ActorGraph {
public:
    static const int MAX_ACTORS = 2000;

    // Custom minimal queue for BFS
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

    // Main graph building and search functions
    static void buildActorGraph(
        const List<Actor>& actorList,
        const List<Movie>& movieList,
        int actorIds[],
        int& actorCount,
        List<int>* adjacencyLists
    );

    static int findActorIndexInArray(int actorId, const int actorIds[], int count);

    // Function to perform BFS and return discovered actors
    static List<int> findConnectedActors(
        int startIndex,
        const List<int>* adjacencyLists,
        int maxDepth = 2
    );
};
#endif // ACTOR_GRAPH_H