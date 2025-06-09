#include "user.h"

// Constructor
User::User()
    : name(""), email(""), age(0) {}

// Getters
std::string User::getName() const { return name; }
std::string User::getEmail() const { return email; }
int User::getAge() const { return age; }
std::vector<Movie> User::getLikedMovies() const { return likedMovies; }
std::vector<Movie> User::getDislikedMovies() const { return dislikedMovies; }

// Setters
void User::setName(const std::string& value) { name = value; }
void User::setEmail(const std::string& value) { email = value; }
void User::setAge(int value) { age = value; }
void User::setLikedMovies(const std::vector<Movie>& movies) { likedMovies = movies; }
void User::setDislikedMovies(const std::vector<Movie>& movies) { dislikedMovies = movies; }

// Utility methods
void User::addLikedMovie(const Movie& movie) {
    likedMovies.push_back(movie);
}

void User::addDislikedMovie(const Movie& movie) {
    dislikedMovies.push_back(movie);
}
