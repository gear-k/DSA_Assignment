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
    static const int MAX_ACTORS = 2000;

    /**
     * @brief A minimal queue used for BFS traversal.
     *
     * The BFSQueue maintains a circular queue of Pair elements, where each Pair
     * contains an actor index and the depth level in the BFS traversal.
     */
    struct BFSQueue {
        /**
         * @brief Structure representing an item in the BFS queue.
         *
         * @param idx The index of the actor in the actorIds array.
         * @param depth The depth level at which the actor was discovered.
         */
        struct Pair {
            int idx;
            int depth;
        };

        Pair data[2000]; ///< Array to store BFS pairs.
        int front;       ///< Index of the front element.
        int rear;        ///< Index of the rear element.
        int count;       ///< Number of elements in the queue.

        /**
         * @brief Constructs a new BFSQueue.
         *
         * Initializes the queue indices and count.
         */
        BFSQueue();

        /**
         * @brief Checks if the queue is empty.
         *
         * @return True if the queue is empty; false otherwise.
         */
        bool isEmpty() const;

        /**
         * @brief Checks if the queue is full.
         *
         * @return True if the queue is full; false otherwise.
         */
        bool isFull() const;

        /**
         * @brief Enqueues an element into the BFSQueue.
         *
         * @param i The actor index to enqueue.
         * @param d The BFS depth of the actor.
         * @return True if the enqueue operation is successful; false if the queue is full.
         */
        bool enqueue(int i, int d);

        /**
         * @brief Dequeues an element from the BFSQueue.
         *
         * The removed element is returned via the out parameter.
         *
         * @param out Reference to a Pair that will hold the dequeued element.
         * @return True if an element was successfully dequeued; false if the queue is empty.
         */
        bool dequeue(Pair& out);
    };

    /**
     * @brief Builds the actor graph using hash tables for actors and movies.
     *
     * This function collects actor IDs from the actorTable and then, for each movie
     * in the movieTable, it finds the indices of the actors that participated in that movie.
     * It then adds each pair of actors (from the same movie) to each other's adjacency list.
     *
     * @param actorTable The hash table containing actors.
     * @param movieTable The hash table containing movies.
     * @param actorIds An array to store actor IDs.
     * @param actorCount Reference to an integer to store the total number of actors.
     * @param adjacencyLists An array of List<int> representing each actor's adjacency list.
     */
    static void buildActorGraph(
        const HashTable<Actor>& actorTable,
        const HashTable<Movie>& movieTable,
        int actorIds[],
        int& actorCount,
        List<int>* adjacencyLists
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
    static List<int> findConnectedActors(
        int startIndex,
        const List<int>* adjacencyLists,
        int maxDepth = 2
    );
};

#endif // ACTOR_GRAPH_H
