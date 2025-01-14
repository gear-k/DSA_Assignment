#ifndef MOVIELIST_H
#define MOVIELIST_H

#include "Movie.h"

class MovieList {
private:
    struct Node {
        Movie movie;
        Node* next;
    };
    Node* head;
    int size;

public:
    MovieList();
    ~MovieList();
    bool add(const Movie& movie);
    bool remove(int index);
    // Non-const version
    Movie* get(int index);

    // Const version
    const Movie* get(int index) const;

    int getLength() const;
    bool isEmpty() const;
    void displayAll() const;
    Movie* findById(int id);



    // --- NEW METHOD ---
    // Sort the list of movies by title (ascending)
    void sortByTitle();
};

#endif
