#include "RatingBST.h"
#include <iostream>

RatingBST::RatingBST() : root(nullptr) {}

RatingBST::~RatingBST() {
    clearTree(root);
}

void RatingBST::clearTree(BSTNode* node) {
    if (!node) return;
    clearTree(node->left);
    clearTree(node->right);
    delete node;
}

void RatingBST::insert(const Movie& movie) {
    insertMovie(root, movie);
}

void RatingBST::insert(const Actor& actor) {
    insertActor(root, actor);
}

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

void RatingBST::inOrderMovies(BSTNode* node) const {
    if (!node) return;
    inOrderMovies(node->left);
    for (const auto& movie : node->movies) {
        movie.displayDetails();
    }
    inOrderMovies(node->right);
}

void RatingBST::inOrderActors(BSTNode* node) const {
    if (!node) return;
    inOrderActors(node->left);
    for (const auto& actor : node->actors) {
        actor.displayDetails();
    }
    inOrderActors(node->right);
}

void RatingBST::displayAllMovies() const {
    inOrderMovies(root);
}

void RatingBST::displayAllActors() const {
    inOrderActors(root);
}

void RatingBST::rangeMovies(BSTNode* node, int minRating, int maxRating) const {
    if (!node) return;
    if (node->rating >= minRating) rangeMovies(node->left, minRating, maxRating);
    if (node->rating >= minRating && node->rating <= maxRating) {
        for (const auto& movie : node->movies) {
            movie.displayDetails();
        }
    }
    if (node->rating <= maxRating) rangeMovies(node->right, minRating, maxRating);
}

void RatingBST::rangeActors(BSTNode* node, int minRating, int maxRating) const {
    if (!node) return;
    if (node->rating >= minRating) rangeActors(node->left, minRating, maxRating);
    if (node->rating >= minRating && node->rating <= maxRating) {
        for (const auto& actor : node->actors) {
            actor.displayDetails();
        }
    }
    if (node->rating <= maxRating) rangeActors(node->right, minRating, maxRating);
}

void RatingBST::displayMoviesInRange(int minRating, int maxRating) const {
    rangeMovies(root, minRating, maxRating);
}

void RatingBST::displayActorsInRange(int minRating, int maxRating) const {
    rangeActors(root, minRating, maxRating);
}
