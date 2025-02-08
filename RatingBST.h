#ifndef RATING_BST_H
#define RATING_BST_H

#include "Movie.h"
#include "Actor.h"
#include "List.h"  // Use the custom List instead of <vector>

/***************************************************************************
 * RatingBST.h
 *
 * Team:            Gearoid, Cedric
 * Group:           1
 * Student IDs:     S10241866, S10241549
 *
 * Features Highlight:
 *   - Implements a Binary Search Tree (BST) to store and manage movies and actors
 *     keyed by their rating.
 *   - Provides operations to insert items, display all elements, and query elements
 *     within a specific rating range using in-order traversal.
 *
 ***************************************************************************/

 /**
  * @brief Structure representing a node in the Rating Binary Search Tree (BST).
  *
  * Each node stores a rating value as the key, along with lists of movies and actors
  * that have that rating. It also contains pointers to the left and right child nodes.
  */
struct BSTNode {
    int rating;           ///< The rating value (key)
    List<Movie> movies;   ///< List of movies with this rating
    List<Actor> actors;   ///< List of actors with this rating
    BSTNode* left;        ///< Pointer to the left child node
    BSTNode* right;       ///< Pointer to the right child node

    /**
     * @brief Constructs a BSTNode with a given rating.
     *
     * Initializes the node's rating and sets the left and right pointers to nullptr.
     *
     * @param r The rating value to be stored in the node.
     */
    BSTNode(int r) : rating(r), left(nullptr), right(nullptr) {}
};

/**
 * @brief A Binary Search Tree (BST) to store and manage movies and actors by rating.
 *
 * The RatingBST provides functions to insert movies and actors into the tree,
 * display all elements in order, and query for elements within a specific rating range.
 */
class RatingBST {
private:
    BSTNode* root;  ///< Pointer to the root of the BST

    /**
     * @brief Recursively inserts a movie into the BST.
     *
     * @param node Reference to the current node pointer.
     * @param movie The movie to insert based on its rating.
     */
    void insertMovie(BSTNode*& node, const Movie& movie);

    /**
     * @brief Recursively inserts an actor into the BST.
     *
     * @param node Reference to the current node pointer.
     * @param actor The actor to insert based on their rating.
     */
    void insertActor(BSTNode*& node, const Actor& actor);

    /**
     * @brief Performs an in-order traversal of the BST to display movie details.
     *
     * @param node The current node in the traversal.
     */
    void inOrderMovies(BSTNode* node) const;

    /**
     * @brief Performs an in-order traversal of the BST to display actor details.
     *
     * @param node The current node in the traversal.
     */
    void inOrderActors(BSTNode* node) const;

    /**
     * @brief Recursively traverses the BST to display movies within a given rating range.
     *
     * @param node The current node in the traversal.
     * @param minRating The minimum rating (inclusive) of the range.
     * @param maxRating The maximum rating (inclusive) of the range.
     */
    void rangeMovies(BSTNode* node, int minRating, int maxRating) const;

    /**
     * @brief Recursively traverses the BST to display actors within a given rating range.
     *
     * @param node The current node in the traversal.
     * @param minRating The minimum rating (inclusive) of the range.
     * @param maxRating The maximum rating (inclusive) of the range.
     */
    void rangeActors(BSTNode* node, int minRating, int maxRating) const;

    /**
     * @brief Recursively deletes all nodes in the BST.
     *
     * Used to free memory when clearing the tree.
     *
     * @param node The current node to delete.
     */
    void clearTree(BSTNode* node);

public:
    /**
     * @brief Constructs a new RatingBST object.
     *
     * Initializes the BST with an empty root.
     */
    RatingBST();

    /**
     * @brief Destroys the RatingBST object.
     *
     * Clears all nodes in the BST to free allocated memory.
     */
    ~RatingBST();

    /**
     * @brief Inserts a movie into the BST based on its rating.
     *
     * @param movie The movie to insert.
     */
    void insert(const Movie& movie);

    /**
     * @brief Inserts an actor into the BST based on their rating.
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
     * @brief Displays movies with ratings within the specified range.
     *
     * Performs an in-order traversal to display movies whose ratings are between
     * minRating and maxRating (inclusive).
     *
     * @param minRating The minimum rating (inclusive).
     * @param maxRating The maximum rating (inclusive).
     */
    void displayMoviesInRange(int minRating, int maxRating) const;

    /**
     * @brief Displays actors with ratings within the specified range.
     *
     * Performs an in-order traversal to display actors whose ratings are between
     * minRating and maxRating (inclusive).
     *
     * @param minRating The minimum rating (inclusive).
     * @param maxRating The maximum rating (inclusive).
     */
    void displayActorsInRange(int minRating, int maxRating) const;
};

#endif // RATING_BST_H
