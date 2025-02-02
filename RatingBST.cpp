/***************************************************************************
 * RatingBST.cpp
 *
 * Team:            Gearoid, Cedric
 * Group:           1
 * Student IDs:     S10241866, S10241549
 *
 * Features Highlight:
 *   - Binary Search Tree (BST) for storing movies and actors by their ratings.
 *   - Insertion and in-order traversal for both movies and actors.
 *   - Range queries to display movies/actors within a given rating range.
 *
 ***************************************************************************/

#include "RatingBST.h"
#include <iostream>

 /**
  * @brief Constructs a new RatingBST object.
  *
  * Initializes the root pointer to nullptr.
  */
RatingBST::RatingBST() : root(nullptr) {}

/**
 * @brief Destroys the RatingBST object.
 *
 * Frees all dynamically allocated nodes in the BST.
 */
RatingBST::~RatingBST() {
    clearTree(root);
}

/**
 * @brief Recursively clears the BST.
 *
 * Deletes all nodes in the subtree rooted at the given node.
 *
 * @param node Pointer to the current BST node.
 */
void RatingBST::clearTree(BSTNode* node) {
    if (!node)
        return;
    clearTree(node->left);
    clearTree(node->right);
    delete node;
}

/**
 * @brief Inserts a movie into the BST based on its rating.
 *
 * @param movie The movie to be inserted.
 */
void RatingBST::insert(const Movie& movie) {
    insertMovie(root, movie);
}

/**
 * @brief Inserts an actor into the BST based on its rating.
 *
 * @param actor The actor to be inserted.
 */
void RatingBST::insert(const Actor& actor) {
    insertActor(root, actor);
}

/**
 * @brief Helper function to insert a movie into the BST.
 *
 * Inserts the movie into the node that corresponds to its rating.
 * If a node with the same rating exists, the movie is added to that node.
 *
 * @param node Reference to the current BST node pointer.
 * @param movie The movie to be inserted.
 */
void RatingBST::insertMovie(BSTNode*& node, const Movie& movie) {
    if (!node) {
        node = new BSTNode(movie.getRating());
        node->movies.push_back(movie);
    }
    else if (movie.getRating() == node->rating) {
        node->movies.push_back(movie);
    }
    else if (movie.getRating() < node->rating) {
        insertMovie(node->left, movie);
    }
    else {
        insertMovie(node->right, movie);
    }
}

/**
 * @brief Helper function to insert an actor into the BST.
 *
 * Inserts the actor into the node that corresponds to its rating.
 * If a node with the same rating exists, the actor is added to that node.
 *
 * @param node Reference to the current BST node pointer.
 * @param actor The actor to be inserted.
 */
void RatingBST::insertActor(BSTNode*& node, const Actor& actor) {
    if (!node) {
        node = new BSTNode(actor.getRating());
        node->actors.push_back(actor);
    }
    else if (actor.getRating() == node->rating) {
        node->actors.push_back(actor);
    }
    else if (actor.getRating() < node->rating) {
        insertActor(node->left, actor);
    }
    else {
        insertActor(node->right, actor);
    }
}

/**
 * @brief Performs an in-order traversal of the BST and displays movie details.
 *
 * @param node Pointer to the current BST node.
 */
void RatingBST::inOrderMovies(BSTNode* node) const {
    if (!node)
        return;
    inOrderMovies(node->left);
    for (const auto& movie : node->movies) {
        movie.displayDetails();
    }
    inOrderMovies(node->right);
}

/**
 * @brief Performs an in-order traversal of the BST and displays actor details.
 *
 * @param node Pointer to the current BST node.
 */
void RatingBST::inOrderActors(BSTNode* node) const {
    if (!node)
        return;
    inOrderActors(node->left);
    for (const auto& actor : node->actors) {
        actor.displayDetails();
    }
    inOrderActors(node->right);
}

/**
 * @brief Displays all movies stored in the BST.
 */
void RatingBST::displayAllMovies() const {
    inOrderMovies(root);
}

/**
 * @brief Displays all actors stored in the BST.
 */
void RatingBST::displayAllActors() const {
    inOrderActors(root);
}

/**
 * @brief Recursively traverses the BST to display movies within a rating range.
 *
 * @param node Pointer to the current BST node.
 * @param minRating The minimum rating (inclusive).
 * @param maxRating The maximum rating (inclusive).
 */
void RatingBST::rangeMovies(BSTNode* node, int minRating, int maxRating) const {
    if (!node)
        return;
    if (node->rating >= minRating)
        rangeMovies(node->left, minRating, maxRating);
    if (node->rating >= minRating && node->rating <= maxRating) {
        for (const auto& movie : node->movies) {
            movie.displayDetails();
        }
    }
    if (node->rating <= maxRating)
        rangeMovies(node->right, minRating, maxRating);
}

/**
 * @brief Recursively traverses the BST to display actors within a rating range.
 *
 * @param node Pointer to the current BST node.
 * @param minRating The minimum rating (inclusive).
 * @param maxRating The maximum rating (inclusive).
 */
void RatingBST::rangeActors(BSTNode* node, int minRating, int maxRating) const {
    if (!node)
        return;
    if (node->rating >= minRating)
        rangeActors(node->left, minRating, maxRating);
    if (node->rating >= minRating && node->rating <= maxRating) {
        for (const auto& actor : node->actors) {
            actor.displayDetails();
        }
    }
    if (node->rating <= maxRating)
        rangeActors(node->right, minRating, maxRating);
}

/**
 * @brief Displays movies whose ratings are within the specified range.
 *
 * @param minRating The minimum rating (inclusive).
 * @param maxRating The maximum rating (inclusive).
 */
void RatingBST::displayMoviesInRange(int minRating, int maxRating) const {
    rangeMovies(root, minRating, maxRating);
}

/**
 * @brief Displays actors whose ratings are within the specified range.
 *
 * @param minRating The minimum rating (inclusive).
 * @param maxRating The maximum rating (inclusive).
 */
void RatingBST::displayActorsInRange(int minRating, int maxRating) const {
    rangeActors(root, minRating, maxRating);
}
