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
 ***************************************************************************/

#include "RatingBST.h"
#include <iostream>

 /**
  * @brief Constructs an empty RatingBST.
  */
RatingBST::RatingBST() : root(nullptr) {}

/**
 * @brief Destructor for RatingBST.
 *
 * Clears the BST and deallocates all nodes.
 */
RatingBST::~RatingBST() {
    clearTree(root);
}

/**
 * @brief Recursively deletes all nodes in the BST.
 *
 * @param node Pointer to the current BST node.
 */
void RatingBST::clearTree(BSTNode* node) {
    if (!node) return;
    clearTree(node->left);
    clearTree(node->right);
    delete node;
}

/**
 * @brief Inserts a movie into the BST based on its rating.
 *
 * This function delegates to the internal helper function insertMovie().
 *
 * @param movie The Movie object to insert.
 */
void RatingBST::insert(const Movie& movie) {
    insertMovie(root, movie);
}

/**
 * @brief Inserts an actor into the BST based on its rating.
 *
 * This function delegates to the internal helper function insertActor().
 *
 * @param actor The Actor object to insert.
 */
void RatingBST::insert(const Actor& actor) {
    insertActor(root, actor);
}

/**
 * @brief Inserts a movie into the correct position in the BST.
 *
 * If a node with the same rating already exists, the movie is added to that node's movie list.
 *
 * @param node Reference to the pointer of the current BST node.
 * @param movie The Movie object to insert.
 */
void RatingBST::insertMovie(BSTNode*& node, const Movie& movie) {
    if (!node) {
        node = new BSTNode(movie.getRating());
        node->movies.add(movie);
    }
    else if (movie.getRating() == node->rating) {
        node->movies.add(movie);
    }
    else if (movie.getRating() < node->rating) {
        insertMovie(node->left, movie);
    }
    else {
        insertMovie(node->right, movie);
    }
}

/**
 * @brief Inserts an actor into the correct position in the BST.
 *
 * If a node with the same rating already exists, the actor is added to that node's actor list.
 *
 * @param node Reference to the pointer of the current BST node.
 * @param actor The Actor object to insert.
 */
void RatingBST::insertActor(BSTNode*& node, const Actor& actor) {
    if (!node) {
        node = new BSTNode(actor.getRating());
        node->actors.add(actor);
    }
    else if (actor.getRating() == node->rating) {
        node->actors.add(actor);
    }
    else if (actor.getRating() < node->rating) {
        insertActor(node->left, actor);
    }
    else {
        insertActor(node->right, actor);
    }
}

/**
 * @brief Recursively traverses the BST and displays movie details in sorted order.
 *
 * Uses in-order traversal to print movies in ascending order by rating.
 *
 * @param node Pointer to the current BST node.
 */
void RatingBST::inOrderMovies(BSTNode* node) const {
    if (!node) return;

    inOrderMovies(node->left);
    node->movies.display([](const Movie& m) {
        m.displayDetails();
        return false; // Continue iteration.
        });
    inOrderMovies(node->right);
}

/**
 * @brief Recursively traverses the BST and displays actor details in sorted order.
 *
 * Uses in-order traversal to print actors in ascending order by rating.
 *
 * @param node Pointer to the current BST node.
 */
void RatingBST::inOrderActors(BSTNode* node) const {
    if (!node) return;

    inOrderActors(node->left);
    node->actors.display([](const Actor& a) {
        a.displayDetails();
        return false; // Continue iteration.
        });
    inOrderActors(node->right);
}

/**
 * @brief Displays all movies stored in the BST in ascending order of rating.
 */
void RatingBST::displayAllMovies() const {
    inOrderMovies(root);
}

/**
 * @brief Displays all actors stored in the BST in ascending order of rating.
 */
void RatingBST::displayAllActors() const {
    inOrderActors(root);
}

/**
 * @brief Recursively traverses the BST and displays movies within the specified rating range.
 *
 * @param node Pointer to the current BST node.
 * @param minRating The minimum rating for display.
 * @param maxRating The maximum rating for display.
 */
void RatingBST::rangeMovies(BSTNode* node, int minRating, int maxRating) const {
    if (!node) return;

    if (node->rating >= minRating) {
        rangeMovies(node->left, minRating, maxRating);
    }

    if (node->rating >= minRating && node->rating <= maxRating) {
        node->movies.display([](const Movie& m) {
            m.displayDetails();
            return false; // Continue iteration.
            });
    }

    if (node->rating <= maxRating) {
        rangeMovies(node->right, minRating, maxRating);
    }
}

/**
 * @brief Recursively traverses the BST and displays actors within the specified rating range.
 *
 * @param node Pointer to the current BST node.
 * @param minRating The minimum rating for display.
 * @param maxRating The maximum rating for display.
 */
void RatingBST::rangeActors(BSTNode* node, int minRating, int maxRating) const {
    if (!node) return;

    if (node->rating >= minRating) {
        rangeActors(node->left, minRating, maxRating);
    }

    if (node->rating >= minRating && node->rating <= maxRating) {
        node->actors.display([](const Actor& a) {
            a.displayDetails();
            return false; // Continue iteration.
            });
    }

    if (node->rating <= maxRating) {
        rangeActors(node->right, minRating, maxRating);
    }
}

/**
 * @brief Displays movies whose ratings fall within the specified range.
 *
 * @param minRating The minimum rating (inclusive).
 * @param maxRating The maximum rating (inclusive).
 */
void RatingBST::displayMoviesInRange(int minRating, int maxRating) const {
    rangeMovies(root, minRating, maxRating);
}

/**
 * @brief Displays actors whose ratings fall within the specified range.
 *
 * @param minRating The minimum rating (inclusive).
 * @param maxRating The maximum rating (inclusive).
 */
void RatingBST::displayActorsInRange(int minRating, int maxRating) const {
    rangeActors(root, minRating, maxRating);
}
