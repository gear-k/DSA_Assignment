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

 // Constructor initializes an empty BST.
RatingBST::RatingBST() : root(nullptr) {}

// Destructor clears the BST.
RatingBST::~RatingBST() {
    clearTree(root);
}

// Recursively deletes all nodes in the BST.
void RatingBST::clearTree(BSTNode* node) {
    if (!node) return;
    clearTree(node->left);
    clearTree(node->right);
    delete node;
}

// Inserts a movie into the BST based on its rating.
void RatingBST::insert(const Movie& movie) {
    insertMovie(root, movie);
}

// Inserts an actor into the BST based on its rating.
void RatingBST::insert(const Actor& actor) {
    insertActor(root, actor);
}

// Inserts a movie into the correct position in the BST based on its rating.
// If a node with the same rating exists, the movie is added to that node.
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

// Inserts an actor into the correct position in the BST based on their rating.
// If a node with the same rating exists, the actor is added to that node.
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

// Recursively traverses the BST and displays movie details in sorted order.
void RatingBST::inOrderMovies(BSTNode* node) const {
    if (!node) return;

    inOrderMovies(node->left);

    node->movies.display([](const Movie& m) {
        m.displayDetails();
        return false; // Continue iteration
        });

    inOrderMovies(node->right);
}

// Recursively traverses the BST and displays actor details in sorted order.
void RatingBST::inOrderActors(BSTNode* node) const {
    if (!node) return;

    inOrderActors(node->left);

    node->actors.display([](const Actor& a) {
        a.displayDetails();
        return false; // Continue iteration
        });

    inOrderActors(node->right);
}

// Displays all movies stored in the BST in ascending order of rating.
void RatingBST::displayAllMovies() const {
    inOrderMovies(root);
}

// Displays all actors stored in the BST in ascending order of rating.
void RatingBST::displayAllActors() const {
    inOrderActors(root);
}

// Recursively traverses the BST and displays movies within the specified rating range.
void RatingBST::rangeMovies(BSTNode* node, int minRating, int maxRating) const {
    if (!node) return;

    if (node->rating >= minRating) {
        rangeMovies(node->left, minRating, maxRating);
    }

    if (node->rating >= minRating && node->rating <= maxRating) {
        node->movies.display([](const Movie& m) {
            m.displayDetails();
            return false;
            });
    }

    if (node->rating <= maxRating) {
        rangeMovies(node->right, minRating, maxRating);
    }
}

// Recursively traverses the BST and displays actors within the specified rating range.
void RatingBST::rangeActors(BSTNode* node, int minRating, int maxRating) const {
    if (!node) return;

    if (node->rating >= minRating) {
        rangeActors(node->left, minRating, maxRating);
    }

    if (node->rating >= minRating && node->rating <= maxRating) {
        node->actors.display([](const Actor& a) {
            a.displayDetails();
            return false;
            });
    }

    if (node->rating <= maxRating) {
        rangeActors(node->right, minRating, maxRating);
    }
}

// Displays movies whose ratings are within the specified range.
void RatingBST::displayMoviesInRange(int minRating, int maxRating) const {
    rangeMovies(root, minRating, maxRating);
}

// Displays actors whose ratings are within the specified range.
void RatingBST::displayActorsInRange(int minRating, int maxRating) const {
    rangeActors(root, minRating, maxRating);
}
