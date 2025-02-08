#ifndef RATING_BST_H
#define RATING_BST_H

#include "Movie.h"
#include "Actor.h"
#include "List.h"  // Use the custom List instead of <vector>

/***************************************************************************
 * Actor.h
 *
 * Team:            Gearoid, Cedric
 * Group:           1
 * Student IDs:     S10241866, S10241549
 *
 * Features Highlight:
 *   - Represents an actor with a fixed-size name buffer, birth year, age, and rating.
 *   - Provides safe copying and assignment operations.
 *   - Includes functions to get and set actor properties as well as display details.
 *
 ***************************************************************************/

// Structure representing a node in the Rating Binary Search Tree (BST).
// Each node stores a rating value as the key, along with lists of movies
// and actors that have that rating. It also has pointers to left and right child nodes.
struct BSTNode {
    int rating;           // The rating value (key)
    List<Movie> movies;   // List of movies with this rating
    List<Actor> actors;   // List of actors with this rating
    BSTNode* left;        // Pointer to the left child
    BSTNode* right;       // Pointer to the right child

    // Constructor to initialize a node with a given rating.
    BSTNode(int r) : rating(r), left(nullptr), right(nullptr) {}
};

// A Binary Search Tree (BST) to store and manage movies and actors by rating.
// Provides functions to insert movies and actors, display all elements,
// and query elements within a specific rating range using in-order traversal.
class RatingBST {
private:
    BSTNode* root;  // Pointer to the root of the BST

    // Recursively inserts a movie into the BST.
    void insertMovie(BSTNode*& node, const Movie& movie);

    // Recursively inserts an actor into the BST.
    void insertActor(BSTNode*& node, const Actor& actor);

    // Performs an in-order traversal of the BST to display movie details.
    void inOrderMovies(BSTNode* node) const;

    // Performs an in-order traversal of the BST to display actor details.
    void inOrderActors(BSTNode* node) const;

    // Recursively traverses the BST to display movies within a rating range.
    void rangeMovies(BSTNode* node, int minRating, int maxRating) const;

    // Recursively traverses the BST to display actors within a rating range.
    void rangeActors(BSTNode* node, int minRating, int maxRating) const;

    // Recursively deletes all nodes in the BST to clear memory.
    void clearTree(BSTNode* node);

public:
    // Constructor initializes the root pointer to nullptr.
    RatingBST();

    // Destructor clears all nodes from the BST.
    ~RatingBST();

    // Inserts a movie into the BST based on its rating.
    void insert(const Movie& movie);

    // Inserts an actor into the BST based on their rating.
    void insert(const Actor& actor);

    // Displays all movies stored in the BST in ascending order of rating.
    void displayAllMovies() const;

    // Displays all actors stored in the BST in ascending order of rating.
    void displayAllActors() const;

    // Displays movies whose ratings are within the specified range.
    void displayMoviesInRange(int minRating, int maxRating) const;

    // Displays actors whose ratings are within the specified range.
    void displayActorsInRange(int minRating, int maxRating) const;
};

#endif // RATING_BST_H
