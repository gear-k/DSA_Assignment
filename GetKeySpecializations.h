// GetKeySpecializations.h
#ifndef GET_KEY_SPECIALIZATIONS_H
#define GET_KEY_SPECIALIZATIONS_H

#include "Actor.h"
#include "Movie.h"
#include "HashTable.h" // Must include after Actor/Movie definitions

// For Actor, we use actorId
template<>
int getKey<Actor>(const Actor& a) {
    return a.getId();
}

// For Movie, we use movieId
template<>
int getKey<Movie>(const Movie& m) {
    return m.getId();
}

#endif // GET_KEY_SPECIALIZATIONS_H
