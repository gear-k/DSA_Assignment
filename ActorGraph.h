#ifndef ACTOR_GRAPH_H
#define ACTOR_GRAPH_H

#include "HashTable.h"
#include "Actor.h"
#include "Movie.h"
#include "List.h"

/***************************************************************************
 * Actor.h
 *
 * Team:            Gearoid, Cedric
 * Group:           1
 * Student IDs:     S10241866, S10241549
 *
 * Features Highlight:
 *   - Represents an actor with a fixed-size name buffer, birth year, age, and rating.
 *   - Provides safe copying and assignment operations.
 *   - Includes functions to get and set actor properties as well as display details.
 *
 ***************************************************************************/

class ActorGraph {
public:
    // Initial capacity for storing actor data
    static const int INITIAL_ACTOR_CAPACITY = 2000;

    // Structure for a queue used in the BFS search
    struct BFSQueue {
        struct Pair {
            int idx;    // Actor index
            int depth;  // Current depth in BFS search
        };

        Pair data[2000];  // Fixed-size array for storing queue data
        int front;        // Index of the front element
        int rear;         // Index of the rear element
        int count;        // Number of elements in the queue

        BFSQueue();          // Constructor to initialize queue
        bool isEmpty() const; // Checks if the queue is empty
        bool isFull() const;  // Checks if the queue is full
        bool enqueue(int i, int d);  // Adds an element to the queue
        bool dequeue(Pair& out);     // Removes an element from the queue
    };

    // Builds the actor graph by linking actors who have worked together in movies
    static void buildActorGraph(
        const HashTable<Actor>& actorTable,  // Table containing actor data
        const HashTable<Movie>& movieTable,  // Table containing movie data
        int*& actorIds,                      // Pointer to dynamically allocated array of actor IDs
        int& actorCount,                     // Total number of actors
        List<int>*& adjacencyLists           // Pointer to adjacency lists of actors
    );

    // Finds the index of an actor in the actorIds array
    static int findActorIndexInArray(int actorId, const int actorIds[], int count);

    // Finds all actors connected to a given actor using a BFS search up to a certain depth
    static List<int> findConnectedActors(
        int startIndex,                  // Index of the starting actor
        const List<int>* adjacencyLists,  // Array of adjacency lists
        int actorCount,                   // Total number of actors
        int maxDepth                       // Maximum depth for the BFS search
    );
};

#endif // ACTOR_GRAPH_H
