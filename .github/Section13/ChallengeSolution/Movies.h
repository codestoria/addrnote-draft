// Section 13 Challenge Solution: a collection of movies.
#ifndef _MOVIES_H_
#define _MOVIES_H_
#include <vector>
#include <string>
#include "Movie.h"

class Movies {
private:
    std::vector<Movie> movies;

public:
    Movies();  // Constructor
    ~Movies(); // Destructor
    // Add a movie if its name is not already present; return whether it was added.
    bool add_movie(std::string name, std::string rating, int watched);

    // Increment the watched count for the named movie; return whether it was found.
    bool increment_watched(std::string name);

    // Display all movies, or a message if the collection is empty.
    void display() const;
};

#endif // _MOVIES_H_
