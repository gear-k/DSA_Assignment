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
 *   - Builds a graph of actors by linking actors who have worked together in movies.
 *   - Provides methods to construct the actor graph, search using BFS, and find
 *     connected actors up to a specified depth.
 *
 ***************************************************************************/

 /**
  * @brief The ActorGraph class builds and manipulates a graph of actors.
  */
class ActorGraph {
public:
    /// Initial capacity for storing actor data
    static const int INITIAL_ACTOR_CAPACITY = 2000;

    /**
     * @brief Structure for a BFS (Breadth-First Search) queue.
     *
     * Used to perform a BFS search on the actor graph.
     */
    struct BFSQueue {
        /**
         * @brief A pair representing an actor's index and the current BFS depth.
         */
        struct Pair {
            int idx;    ///< Actor index in the actor graph.
            int depth;  ///< Current depth in the BFS search.
        };

        Pair data[2000];  ///< Fixed-size array for storing queue elements.
        int front;        ///< Index of the front element.
        int rear;         ///< Index of the rear element.
        int count;        ///< Current number of elements in the queue.

        /**
         * @brief Constructs a new BFSQueue object and initializes its members.
         */
        BFSQueue();

        /**
         * @brief Checks if the queue is empty.
         * @return true if the queue has no elements, false otherwise.
         */
        bool isEmpty() const;

        /**
         * @brief Checks if the queue is full.
         * @return true if the queue cannot accept more elements, false otherwise.
         */
        bool isFull() const;

        /**
         * @brief Enqueues an element into the queue.
         *
         * @param i The actor index to enqueue.
         * @param d The BFS depth associated with this actor.
         * @return true if the element was successfully enqueued, false if the queue is full.
         */
        bool enqueue(int i, int d);

        /**
         * @brief Dequeues an element from the queue.
         *
         * @param out Reference to a Pair where the dequeued element will be stored.
         * @return true if an element was successfully dequeued, false if the queue is empty.
         */
        bool dequeue(Pair& out);
    };

    /**
     * @brief Builds the actor graph by linking actors who have worked together in movies.
     *
     * Iterates through the movie table to determine which actors have co-starred,
     * constructing adjacency lists that represent those connections.
     *
     * @param actorTable A hash table containing actor data.
     * @param movieTable A hash table containing movie data.
     * @param actorIds Reference to a pointer that will be assigned a dynamically allocated array of actor IDs.
     * @param actorCount Reference to an integer that will hold the total number of actors.
     * @param adjacencyLists Reference to a pointer that will be assigned the dynamically allocated adjacency lists.
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
     * Searches for the actor with the given ID in the array.
     *
     * @param actorId The unique ID of the actor.
     * @param actorIds The array of actor IDs.
     * @param count The total number of actors in the array.
     * @return The index of the actor if found; otherwise, -1.
     */
    static int findActorIndexInArray(int actorId, const int actorIds[], int count);

    /**
     * @brief Finds all actors connected to a specified actor using BFS.
     *
     * Performs a breadth-first search (BFS) on the actor graph starting from the given index,
     * collecting indices of all actors connected up to a maximum search depth.
     *
     * @param startIndex The index of the starting actor.
     * @param adjacencyLists The array of adjacency lists representing the graph.
     * @param actorCount The total number of actors in the graph.
     * @param maxDepth The maximum depth for the BFS search.
     * @return A List<int> containing the indices of all connected actors found within the specified depth.
     */
    static List<int> findConnectedActors(
        int startIndex,
        const List<int>* adjacencyLists,
        int actorCount,
        int maxDepth
    );
};

#endif // ACTOR_GRAPH_H
