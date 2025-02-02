#ifndef RATING_BST_H
#define RATING_BST_H

#include <vector>
#include "Movie.h"
#include "Actor.h"

/***************************************************************************
 * RatingBST.h
 *
 * Team:            Gearoid, Cedric
 * Group:           1
 * Student IDs:     S10241866, S10241549
 *
 * Features Highlight:
 *   - Binary Search Tree (BST) to store movies and actors by rating.
 *   - Insertion, in-order traversal, and range queries for both movies and actors.
 *
 ***************************************************************************/

 /**
  * @brief Structure representing a node in the Rating Binary Search Tree.
  *
  * Each node stores a rating value as key along with vectors of movies
  * and actors that have that rating. It also has pointers to the left and right
  * child nodes.
  */
struct BSTNode {
    int rating;                     // Key: Rating
    std::vector<Movie> movies;      // Movies with this rating
    std::vector<Actor> actors;      // Actors with this rating
    BSTNode* left;
    BSTNode* right;

    /**
     * @brief Constructs a BSTNode with a specified rating.
     *
     * @param r The rating key for the node.
     */
    BSTNode(int r) : rating(r), left(nullptr), right(nullptr) {}
};

/**
 * @brief RatingBST class to store and manage movies and actors by rating.
 *
 * Provides functions to insert movies and actors, display all elements,
 * and query elements within a specific rating range using in-order traversal.
 */
class RatingBST {
private:
    BSTNode* root;

    /**
     * @brief Recursively inserts a movie into the BST.
     *
     * @param node Reference to the current BST node pointer.
     * @param movie The movie to insert.
     */
    void insertMovie(BSTNode*& node, const Movie& movie);

    /**
     * @brief Recursively inserts an actor into the BST.
     *
     * @param node Reference to the current BST node pointer.
     * @param actor The actor to insert.
     */
    void insertActor(BSTNode*& node, const Actor& actor);

    /**
     * @brief Performs an in-order traversal of the BST and displays movie details.
     *
     * @param node Pointer to the current BST node.
     */
    void inOrderMovies(BSTNode* node) const;

    /**
     * @brief Performs an in-order traversal of the BST and displays actor details.
     *
     * @param node Pointer to the current BST node.
     */
    void inOrderActors(BSTNode* node) const;

    /**
     * @brief Recursively traverses the BST to display movies within a rating range.
     *
     * @param node Pointer to the current BST node.
     * @param minRating The minimum rating (inclusive).
     * @param maxRating The maximum rating (inclusive).
     */
    void rangeMovies(BSTNode* node, int minRating, int maxRating) const;

    /**
     * @brief Recursively traverses the BST to display actors within a rating range.
     *
     * @param node Pointer to the current BST node.
     * @param minRating The minimum rating (inclusive).
     * @param maxRating The maximum rating (inclusive).
     */
    void rangeActors(BSTNode* node, int minRating, int maxRating) const;

    /**
     * @brief Recursively deletes all nodes in the BST.
     *
     * @param node Pointer to the current BST node.
     */
    void clearTree(BSTNode* node);

public:
    /**
     * @brief Constructs a new RatingBST object.
     *
     * Initializes the root pointer to nullptr.
     */
    RatingBST();

    /**
     * @brief Destroys the RatingBST object.
     *
     * Clears all nodes from the BST.
     */
    ~RatingBST();

    /**
     * @brief Inserts a movie into the BST.
     *
     * @param movie The movie to insert.
     */
    void insert(const Movie& movie);

    /**
     * @brief Inserts an actor into the BST.
     *
     * @param actor The actor to insert.
     */
    void insert(const Actor& actor);

    /**
     * @brief Displays all movies stored in the BST in ascending order of rating.
     */
    void displayAllMovies() const;

    /**
     * @brief Displays all actors stored in the BST in ascending order of rating.
     */
    void displayAllActors() const;

    /**
     * @brief Displays movies whose ratings are within the specified range.
     *
     * @param minRating The minimum rating (inclusive).
     * @param maxRating The maximum rating (inclusive).
     */
    void displayMoviesInRange(int minRating, int maxRating) const;

    /**
     * @brief Displays actors whose ratings are within the specified range.
     *
     * @param minRating The minimum rating (inclusive).
     * @param maxRating The maximum rating (inclusive).
     */
    void displayActorsInRange(int minRating, int maxRating) const;
};

#endif // RATING_BST_H
