#ifndef USER_H
#define USER_H

#include <string>
#include <vector>
#include "movie.h"

class User {
public:
    // Constructor
    User();

    // Getters
    std::string getName() const;
    std::string getEmail() const;
    int getAge() const;
    std::vector<Movie> getLikedMovies() const;
    std::vector<Movie> getDislikedMovies() const;

    // Setters
    void setName(const std::string& value);
    void setEmail(const std::string& value);
    void setAge(int value);
    void setLikedMovies(const std::vector<Movie>& movies);
    void setDislikedMovies(const std::vector<Movie>& movies);

    // Utilities
    void addLikedMovie(const Movie& movie);
    void addDislikedMovie(const Movie& movie);

private:
    std::string name;
    std::string email;
    int age;
    std::vector<Movie> likedMovies;
    std::vector<Movie> dislikedMovies;
};

#endif // USER_H
