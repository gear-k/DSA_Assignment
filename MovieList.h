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

    Node* mergeSortedLists(Node* left, Node* right);
    Node* mergeSort(Node* node);

public:
    MovieList();
    ~MovieList();
    bool add(const Movie& movie);
    bool remove(int index);
    Movie* findById(int id);
    Movie* get(int index);               // Non-const version
    const Movie* get(int index) const;   // Const version

    int getLength() const;
    bool isEmpty() const;
    void displayAll() const;
    void sortByTitle(); // Sort movies by title
};

#endif
