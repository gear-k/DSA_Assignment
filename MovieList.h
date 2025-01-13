#pragma once
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
    Movie* get(int index);
    int getLength() const;
    bool isEmpty() const;
    void displayAll() const;
};
