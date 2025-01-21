#ifndef RATING_BST_H
#define RATING_BST_H

#include <vector>
#include "Movie.h"
#include "Actor.h"

struct BSTNode {
    int rating;                     // Key: Rating
    std::vector<Movie> movies;      // Movies with this rating
    std::vector<Actor> actors;      // Actors with this rating
    BSTNode* left;
    BSTNode* right;

    BSTNode(int r) : rating(r), left(nullptr), right(nullptr) {}
};

class RatingBST {
private:
    BSTNode* root;

    void insertMovie(BSTNode*& node, const Movie& movie);
    void insertActor(BSTNode*& node, const Actor& actor);
    void inOrderMovies(BSTNode* node) const;
    void inOrderActors(BSTNode* node) const;
    void rangeMovies(BSTNode* node, int minRating, int maxRating) const;
    void rangeActors(BSTNode* node, int minRating, int maxRating) const;
    void clearTree(BSTNode* node);

public:
    RatingBST();
    ~RatingBST();

    void insert(const Movie& movie);
    void insert(const Actor& actor);
    void displayAllMovies() const;
    void displayAllActors() const;
    void displayMoviesInRange(int minRating, int maxRating) const;
    void displayActorsInRange(int minRating, int maxRating) const;
};

#endif // RATING_BST_H
