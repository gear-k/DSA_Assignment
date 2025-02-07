#ifndef ACTOR_GRAPH_H
#define ACTOR_GRAPH_H

#include "HashTable.h"
#include "Actor.h"
#include "Movie.h"
#include "List.h"

/***************************************************************************
 * ActorGraph.h
 *
 * Team:            Gearoid, Cedric
 * Group:           1
 * Student IDs:     S10241866, S10241549
 *
 * Features Highlight:
 *   - Builds an adjacency graph representing which actors have worked together.
 *   - Uses hash tables for fast look-up of actors and movies.
 *   - Performs Breadth-First Search (BFS) to discover connected actors up to a specified depth.
 *
 ***************************************************************************/

 /**
  * @brief The ActorGraph class builds and traverses an actor graph.
  *
  * It creates an adjacency list indicating which actors have co-starred in movies.
  * The class provides a BFS-based function to find all actors connected to a given actor
  * up to a given maximum depth.
  */
class ActorGraph {
public:
    // (The MAX_ACTORS constant is no longer a hard limit for actor IDs.)
    static const int INITIAL_ACTOR_CAPACITY = 2000;

    // --- BFSQueue (remains unchanged) ---
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

    /**
     * @brief Builds the actor graph using hash tables for actors and movies.
     *
     * This function collects actor IDs from the actorTable and then, for each movie
     * in the movieTable, finds the indices of the actors that participated in that movie.
     * It then adds each pair of actors (from the same movie) to each other's adjacency list.
     *
     * @param actorTable The hash table containing actors.
     * @param movieTable The hash table containing movies.
     * @param actorIds A reference to a pointer that will point to a dynamically allocated array of actor IDs.
     * @param actorCount Reference to an integer to store the total number of actors.
     * @param adjacencyLists A reference to a pointer to a dynamically allocated array of List<int>
     *                       (each element representing an actor’s adjacency list).
     */
    static void buildActorGraph(
        const HashTable<Actor>& actorTable,
        const HashTable<Movie>& movieTable,
        int*& actorIds,
        int& actorCount,
        List<int>*& adjacencyLists
    );

    /**
     * @brief Finds the index of an actor in the actorIds array.
     *
     * Searches through the actorIds array to find the index corresponding to the given actorId.
     *
     * @param actorId The actor's ID to search for.
     * @param actorIds The array containing actor IDs.
     * @param count The number of actor IDs in the array.
     * @return The index of the actor if found; -1 if not found.
     */
    static int findActorIndexInArray(int actorId, const int actorIds[], int count);

   

    /**
     * @brief Finds all actors connected to the starting actor within a given maximum depth.
     *
     * Performs a breadth-first search (BFS) starting from the actor at startIndex, traversing
     * the actor graph defined by the adjacencyLists. The search continues until the maximum depth
     * is reached. Discovered actor indices are returned in a list.
     *
     * @param startIndex The index of the starting actor.
     * @param adjacencyLists The array of adjacency lists for all actors.
     * @param maxDepth The maximum depth for BFS traversal (default is 2).
     * @return A List<int> containing the indices of all connected actors discovered.
     */
    static List<int> findConnectedActors(int startIndex, const List<int>* adjacencyLists, int actorCount, int maxDepth);
};

#endif // ACTOR_GRAPH_H
