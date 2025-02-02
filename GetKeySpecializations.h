// GetKeySpecializations.h
#ifndef GET_KEY_SPECIALIZATIONS_H
#define GET_KEY_SPECIALIZATIONS_H

/***************************************************************************
 * GetKeySpecializations.h
 *
 * Team:            Gearoid, Cedric
 * Group:           1
 * Student IDs:     S10241866, S10241549
 *
 * Features Highlight:
 *   - Provides template specializations for getKey() function.
 *   - Retrieves the unique integer key for Actor and Movie objects.
 *
 * Note:
 *   - Must be included after the Actor.h and Movie.h definitions.
 *
 ***************************************************************************/

#include "Actor.h"
#include "Movie.h"
#include "HashTable.h" // Must include after Actor/Movie definitions

 /**
  * @brief Specialization of getKey for Actor.
  *
  * Retrieves the unique actor ID from an Actor object.
  *
  * @param a The Actor object.
  * @return The actor's unique ID.
  */
template<>
int getKey<Actor>(const Actor& a) {
    return a.getId();
}

/**
 * @brief Specialization of getKey for Movie.
 *
 * Retrieves the unique movie ID from a Movie object.
 *
 * @param m The Movie object.
 * @return The movie's unique ID.
 */
template<>
int getKey<Movie>(const Movie& m) {
    return m.getId();
}

#endif // GET_KEY_SPECIALIZATIONS_H
